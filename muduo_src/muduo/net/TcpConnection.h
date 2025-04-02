// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_TCPCONNECTION_H
#define MUDUO_NET_TCPCONNECTION_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/StringPiece.h"
#include "muduo/base/Types.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/InetAddress.h"

#include <memory>
#include <atomic>       // 修改 state_

#include <boost/any.hpp>

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

namespace muduo
{
namespace net
{

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
/// This is an interface class, so don't expose too much details.
class TcpConnection : noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
 public:
    TcpConnection(EventLoop* loop,
                const string& name,
                int sockfd,
                const InetAddress& localAddr,
                const InetAddress& peerAddr);
    ~TcpConnection();

    // 连接相关信息获取
    EventLoop* getLoop() const { return loop_; }
    const string& name() const { return name_; }
    const InetAddress& localAddress() const { return localAddr_; }
    const InetAddress& peerAddress() const { return peerAddr_; }
    bool connected() const { return state_ == kConnected; }
    bool disconnected() const { return state_ == kDisconnected; }
    bool getTcpInfo(struct tcp_info*) const;
    string getTcpInfoString() const;

    // 发送数据
    //void send(const std::string& message); // C++11
    void send(const void* message, int len);
    void send(const StringPiece& message);
    void send(Buffer* message);  // this one will swap data
    //void send(Buffer&& message); // C++11

    // 关闭连接
    void shutdown();
    void forceClose();
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);

    // 读控制
    void startRead();
    void stopRead();
    bool isReading() const { return reading_; }; // NOT thread safe, may race with start/stopReadInLoop

    // 上下文获取
    void setContext(const boost::any& context)
    { context_ = context; }
    const boost::any& getContext() const
    { return context_; }
    boost::any* getMutableContext()
    { return &context_; }

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }

    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    { writeCompleteCallback_ = cb; }

    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
    { highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark; }

    /// Advanced interface
    Buffer* inputBuffer()
    { return &inputBuffer_; }

    Buffer* outputBuffer()
    { return &outputBuffer_; }

    // 通知TcpServer/TcpClient移除它们所持有的TcpConnectionPtr
    void setCloseCallback(const CloseCallback& cb)
    { closeCallback_ = cb; }

    // 连接建立后 注册事件到poller中进行监听（Channel_处理）
    void connectEstablished();
    // channel_
    void connectDestroyed();


 private:
    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
    void setState(StateE s) { state_ = s; }
    const char* stateToString() const;

    // 这些事件被触发后，由 Channel 调用执行
    void handleRead(Timestamp receiveTime);	/* 可读事件通过回调 MessageCallback 传达给客户 */
    void handleWrite();						// 自己处理可写事件
    void handleClose();
    void handleError();
  
    // void sendInLoop(string&& message);
    void sendInLoop(const StringPiece& message);
    void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
    // void shutdownAndForceCloseInLoop(double seconds);
  
    void forceCloseInLoop();
    void startReadInLoop();
    void stopReadInLoop();



    EventLoop* loop_;     // 当前连接所属的一个 subLoop，conn 由子loop管理
    const string name_;
    //StateE state_;  // FIXME: use atomic variable
    std::atomic_int state_;
    bool reading_;

    // we don't expose those classes to client.
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    // 下面的回调在创建新连接将其封装为conn对象时，由TcpServer提供初始化
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;

    CloseCallback closeCallback_;         // 供TcpServer、TcpClient使用，通知它们移除所持有的TcpConnectionPtr
  
    size_t highWaterMark_;        // 数据量高水位限制
    // 属于连接用户的缓冲区
    Buffer inputBuffer_;      // onMessage() 回调中读取 
    Buffer outputBuffer_;     // send()发送数据时使用 当TCP发送缓冲区达到高水位限制时，将待发送数据暂存在这里

    boost::any context_;			/* 保存用户上下文，例如 FILE* */
};  // end TcpConnecion class


typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;    // shared_ptr管理TcpConnection

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_TCPCONNECTION_H
