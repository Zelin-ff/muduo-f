// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOPTHREAD_H
#define MUDUO_NET_EVENTLOOPTHREAD_H

#include "muduo/base/Condition.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"

namespace muduo
{
namespace net
{

class EventLoop;

class EventLoopThread : noncopyable
{
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                  const string& name = string());
  ~EventLoopThread();
  
  EventLoop* startLoop();	    // 创建新的线程，并在内部实例化另一个 EventLoop 后并将其返回

 private:
  void threadFunc();	// 线程启动后在其内部执行，用于创建 loop 并启动

  EventLoop* loop_ GUARDED_BY(mutex_);	// 指向线程中创建的 loop
  bool exiting_;
  Thread thread_;						// 初始化时绑定了线程执行函数threadFunc()
  MutexLock mutex_;
  Condition cond_ GUARDED_BY(mutex_);	// "线程间同步"用来通知
  ThreadInitCallback callback_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_EVENTLOOPTHREAD_H

