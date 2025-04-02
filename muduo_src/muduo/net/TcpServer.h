// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_TCPSERVER_H
#define MUDUO_NET_TCPSERVER_H

#include "muduo/base/Atomic.h"
#include "muduo/base/Types.h"
#include "muduo/net/TcpConnection.h"

#include <map>

namespace muduo
{
namespace net
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

/// TCP server, 支持单线程和线程池
///
/// 属于接口类，尽量不要暴漏内部实现细节

class TcpServer : noncopyable
{
public:

    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    enum Option
    {
    kNoReusePort,
    kReusePort,
    };

    //TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    TcpServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg,
            Option option = kNoReusePort);
    ~TcpServer();  // force out-line dtor, for std::unique_ptr members.

    const string& ipPort() const { return ipPort_; }
    const string& name() const { return name_; }
    EventLoop* getLoop() const { return loop_; }

    //- 0 默认值，表示 all I/O in loop's thread, 不创建任何线程
    //- 1 表示 all I/O 工作在创建的另一个线程
    //- N 表示线程池包含 N 个线程，新建立的连接被分发到其中一个（轮询机制实现）
    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback& cb)
    { threadInitCallback_ = cb; }
    /// .start()后开始工作
    std::shared_ptr<EventLoopThreadPool> threadPool()
    { return threadPool_; }

    // 启动Acceptor监听
    void start();

    // 下列回调函数的设置都是 not thread safe
    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    { writeCompleteCallback_ = cb; }


 private:

    // 封装新的连接为conn对象，线程池中取出一个IO线程并且将新的连接放入
    void newConnection(int sockfd, const InetAddress& peerAddr);
    // 移除连接
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    typedef std::map<string, TcpConnectionPtr> ConnectionMap;	/* 保存存活的TcpConnectionPtr对象 */

    EventLoop* loop_;         // the acceptor loop 只负责建立连接
    const string ipPort_;
    const string name_;
    std::unique_ptr<Acceptor> acceptor_;      // unique_ptr避免Accptor暴露
    std::shared_ptr<EventLoopThreadPool> threadPool_;

    /* TcpServer 的回调成员，都传给了封装新连接后的 conn 对象 */
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;
    AtomicInt32 started_;
    // always in loop thread
    int nextConnId_;
    ConnectionMap connections_;           // std::map<string, TcpConnectionPtr> 保存连接
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_TCPSERVER_H
