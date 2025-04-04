// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <functional>

#include "muduo/net/Channel.h"
#include "muduo/net/Socket.h"

namespace muduo
{
namespace net
{

class EventLoop;
class InetAddress;	/* 封装了struct sockaddr_in，可以自动转换字节序 */

///
/// Acceptor of incoming TCP connections.
///
class Acceptor : noncopyable
{
 public:
  typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  ~Acceptor();
  // 由TcpServer构造时赋值给Acceptor，然后Acceptor再给Channel
  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  // 由TcpServer::start()启动，通过acceptSocket_开启监听，然后将acceptChannel_注册（监听新用户的连接请求）
  void listen();

  bool listening() const { return listening_; }

  // Deprecated, use the correct spelling one above.
  // Leave the wrong spelling here in case one needs to grep it for error messages.
  // bool listenning() const { return listening(); }

 private:
  // 由Channel回调时执行，实现::accept()，然后执行newConnectionCallback_(TcpServer::newConnection)
  void handleRead();

  EventLoop* loop_;			// 所属于的 loop，就是主Reactor
  Socket acceptSocket_;		// 保存创建的非阻塞 socket fd
  Channel acceptChannel_;	// 负责 socket 上事件的注册、回调
  NewConnectionCallback newConnectionCallback_;	// 将新的连接分发到一个子IO线程下的 EventLoop
  bool listening_;
  int idleFd_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_ACCEPTOR_H
