#include "muduo/base/Logging.h"
#include "muduo/net/Endian.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"

#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class TimeClient : noncopyable
{
 public:
  TimeClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop),
      client_(loop, serverAddr, "TimeClient")
  {
    client_.setConnectionCallback(
        std::bind(&TimeClient::onConnection, this, _1));
    client_.setMessageCallback(
        std::bind(&TimeClient::onMessage, this, _1, _2, _3));
    // client_.enableRetry();
  }

  void connect()
  {
    client_.connect();
  }

 private:

  EventLoop* loop_;
  TcpClient client_;

  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (!conn->connected())
    {
      loop_->quit();
    }
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
  {
    if (buf->readableBytes() >= sizeof(int32_t))
    {
      const void* data = buf->peek();	/* 获取缓冲区数据的起始地址，进行类型转换 */
      int32_t be32 = *static_cast<const int32_t*>(data);
      buf->retrieve(sizeof(int32_t));	/* 从缓冲区移除已读取的数据 */
      time_t time = sockets::networkToHost32(be32);
      Timestamp ts(implicit_cast<uint64_t>(time) * Timestamp::kMicroSecondsPerSecond);
      LOG_INFO << "Server time = " << time << ", " << ts.toFormattedString();
    }
    else
    {
      LOG_INFO << conn->name() << " no enough data " << buf->readableBytes()
               << " at " << receiveTime.toFormattedString();
    }
  }
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    EventLoop loop;
    InetAddress serverAddr(argv[1], 2037);

    TimeClient timeClient(&loop, serverAddr);
    timeClient.connect();
    loop.loop();
  }
  else
  {
    printf("Usage: %s host_ip\n", argv[0]);
  }
}

