// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Timestamp.h"

#include <functional>
#include <memory>

namespace muduo
{
namespace net
{

class EventLoop;

///
///  A selectable I/O channel.
/// Channel 类不拥有 file descriptor（可以是：eventfd，timerfd，signalfd）
/// 
class Channel : noncopyable
{
 public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(Timestamp)> ReadEventCallback;

  Channel(EventLoop* loop, int fd);
  ~Channel();
  // 资源（TcpConnection）安全地回调处理
  void handleEvent(Timestamp receiveTime);

  /* 设置不同事件类型的回调函数 */
  void setReadCallback(ReadEventCallback cb)
  { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb)
  { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback cb)
  { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback cb)
  { errorCallback_ = std::move(cb); }

  // 将Channel对象绑定到所有者对象（managed by shared_ptr）
  // 防止在执行 handleEvent() 过程中所有者对象被摧毁
  void tie(const std::shared_ptr<void>&);

  int fd() const { return fd_; }
  int events() const { return events_; }
  void set_revents(int revt) { revents_ = revt; } // used by pollers
  // int revents() const { return revents_; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }

  /* epoll_ctl()注册、移除 fd 上的感兴趣事件 */
  void enableReading() { events_ |= kReadEvent; update(); }
  void disableReading() { events_ &= ~kReadEvent; update(); }
  void enableWriting() { events_ |= kWriteEvent; update(); }
  void disableWriting() { events_ &= ~kWriteEvent; update(); }
  void disableAll() { events_ = kNoneEvent; update(); }
  bool isWriting() const { return events_ & kWriteEvent; }	// 判断是否可写
  bool isReading() const { return events_ & kReadEvent; }	// 判断是否可读

  // for Poller
  int index() { return index_; }
  void set_index(int idx) { index_ = idx; }

  // for debug
  string reventsToString() const;
  string eventsToString() const;

  void doNotLogHup() { logHup_ = false; }

  EventLoop* ownerLoop() { return loop_; }
  // loop_将channel从poller中移除
  void remove();

 private:
  static string eventsToString(int fd, int ev);

  void update();
  // 根据触发事件的类型回调处理函数
  void handleEventWithGuard(Timestamp receiveTime);

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;
  const int  fd_;	
  int        events_;		// 添加到监听的感兴趣事件：IO事件、定时器任务
  int        revents_;		// 监听到触发的事件，通过epoll、poll获得
  int        index_;		// used by Poller.
  bool       logHup_;

  std::weak_ptr<void> tie_;	// 指向绑定的资源，思考弱引用的含义？
  bool tied_;
  bool eventHandling_;
  bool addedToLoop_;
  ReadEventCallback readCallback_;		// 需要一个参数 Timestamp
  EventCallback writeCallback_;
  EventCallback closeCallback_;			// 调用 TcpConnection::handleClose()
  EventCallback errorCallback_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_CHANNEL_H
