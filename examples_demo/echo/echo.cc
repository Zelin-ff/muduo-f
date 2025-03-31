//#include <functional>
#include "muduo/net/TcpServer.h"

#include "muduo/base/Atomic.h"		// 原子操作，确保线程安全
#include "muduo/base/FileUtil.h"
#include "muduo/base/Logging.h"
#include "muduo/base/ProcessInfo.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

#include <utility>
#include <stdio.h>
#include <unistd.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using namespace muduo;
using namespace muduo::net;

int numThreads = 0;
int maxConnections = 2;

class EchoServer
{
public:
	EchoServer(EventLoop* loop,
		const InetAddress& listenAddr,
		int maxConnections)
		: loop_(loop),
		  server_(loop, listenAddr, "EchoServer"),
		  numConnected_(0),
		  kMaxConnections_(maxConnections)
	{
		server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
		server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
		server_.setThreadNum(numThreads);

		loop_->runEvery(5.0, std::bind(&EchoServer::printThroughput, this));
	}

	void start() {
		LOG_INFO << "starting " << numThreads << "threads.";
		server_.start();
	}

private:
	void onConnection(const TcpConnectionPtr& conn) {
		LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
			<< conn->localAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");

		if (conn->connected()) {
			//++numConnected_;
			//if (numConnected_ > kMaxConnections_) {
			//	conn->shutdown();
			//	conn->forceCloseWithDelay(3.0);
			//}
			connections_.increment();
		}
		else {
			//--numConnected_;
			connections_.decrement();
		}
		//LOG_INFO << "numConnected_ = " << numConnected_;
	}

	void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
		//string msg(buf->retrieveAllAsString());
		//LOG_INFO << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
		//conn->send(msg);

		size_t len = buf->readableBytes();			// 统计数据长度
		transferredBytes_.addAndGet(len);
		receivedMessages_.incrementAndGet();		// 统计消息接收次数
		conn->send(buf);
	}
	// 打印吞吐量
	void printThroughput() {
		Timestamp endTime = Timestamp::now();
		double bytes = static_cast<double>(transferredBytes_.getAndSet(0));
		int msgs = receivedMessages_.getAndSet(0);
		double bytesPerMsg = msgs > 0 ? bytes / msgs : 0;
		double time = timeDifference(endTime, startTime_);
		printf("%.3f MiB/s %.2f Kilo Msgs/s %.2f bytes per msg, ",
			bytes / time / 1024 / 1024,
			static_cast<double>(msgs) / time / 1000,
			bytesPerMsg);

		printConnection();
		fflush(stdout);
		startTime_ endTime;
	}

	void printConnection() {
		string procStatus = ProcessInfo::procStatus();	// 进程状态
		printf("%d conn, files %d, VmSize %ld KiB, RSS %ld KiB, ",
			connections_.get(),
			ProcessInfo::openedFiles(),
			getLong(procStatus, "VmSize:"),
			getLong(procStatus, "VmRSS:"));
		// 解析
		string meminfo;
		FileUtil::readFile("/proc/meminfo", 65536, &meminfo);
		long total_kb = getLong(meminfo, "MemTotal:");
		long free_kb = getLong(meminfo, "MemFree:");
		long buffers_kb = getLong(meminfo, "Buffers:");
		long cached_kb = getLong(meminfo, "Cached:");
		printf("system memory used %ld KiB\n",
			total_kb - free_kb - buffers_kb - cached_kb);
	}
	// 提取 key 后面的 long类型整数值
	long getLong(const string& procStatus, const char* key) {
		long result = 0;
		size_t pos = procStatus.find(key);
		if (pos != string::npos) {	// string::npos 通常输出size_t最大值，表示无法查找
			result = ::atol(procStatus.c_str() + pos + strlen(key));
		}
		return result;
	}

	EventLoop* loop_;
	TcpServer server_;
	AtomicInt32 connections_;
	AtomicInt32 receivedMessages_;
	AtomicInt64 transferredBytes_;
	int numConnected_;
	const int kMaxConnections_;		// 限制连接数量
	Timestamp startTime_;
};

int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid()
		<< ", tid = " << CurrentThread::tid()
		<< ", max files = " << ProcessInfo::maxOpenFiles();
	Logger::setLogLevel(Logger::WARN);
	if (argc > 1) {
		numThreads = atoi(argv[1]);
	}

	EventLoop loop;
	InetAddress listenAddr(2025);
	EchoServer server(&loop, listenAddr);

	server.start();

	loop.loop();
}