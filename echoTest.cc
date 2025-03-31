#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Timestamp.h"

#include <unistd.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class EchoServer
{
public:
	EchoServer(muduo::net::EventLoop* loop, muduo::net::InetAddress& listenAddr)
		: server_(loop, listenAddr, "EchoServer")
	{
		server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
		server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
	}

	void start() {
		server_.start();
	}

private:
	void onConnection(muduo::net::TcpConnectionPtr& conn) {
		/* EchoServer - 127.0.0.1:8888 -> 0.0.0.0:8888 is UP */
		LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
			<< conn->localAddress().toIpPort() << " is "
			<< (conn->connected() ? "UP" : "DOWN");
	}

	void onMessage(const muduo::net::TcpConnectionPtr& conn,
		muduo::net::Buffer* buf,
		muduo::Timestamp time) {
		muduo::string msg(buf->retrieveAllAsString());
		LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
			<< "data received at " << time.toString();
		conn->send(msg);
	}

	muduo::net::TcpServer server_;
};

int main()
{
	LOG_INFO << "pid = " << getpid();
	muduo::net::EventLoop loop;
	muduo::net::InetAddress listenAddr(8888);
	EchoServer server(&loop, listenAddr);
	server.start();
	loop.loop();
}