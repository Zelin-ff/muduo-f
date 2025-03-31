// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/EventLoopThread.h"

#include "muduo/net/EventLoop.h"

using namespace muduo;
using namespace muduo::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const string& name)
  : loop_(NULL),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this), name),
    mutex_(),
    cond_(mutex_),
    callback_(cb)
{
    /* 在没有初始化构造函数参数为空时：
       主要是将 threadFunc() 函数传递给 Thread 对象
       进一步 startLoop()->调用Thread::start() 来创建一个线程，并执行这个 threadFunc() */
}

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  if (loop_ != NULL) // not 100% race-free, eg. threadFunc could be running callback_.
  {
    // still a tiny chance to call destructed object, if threadFunc exits just now.
    // but when EventLoopThread destructs, usually programming is exiting anyway.
    loop_->quit();
    thread_.join();
  }
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();          // 创建线程 pthread_create，执行 threadFunc()

  EventLoop* loop = NULL;       
  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL)	// 阻塞条件变量... 直到线程内的 loop 创建成功
    {
      cond_.wait();
    }
    loop = loop_;			// 在 threadFunc()中的 loop 启动循环之前就获取到了 loop
  }

  return loop;              // 返回新线程下创建的 loop
}

void EventLoopThread::threadFunc()
{
  EventLoop loop;		// 新线程下重新创建的 loop 为局部对象，生命期直到 .quit()

  if (callback_)
  {
    callback_(&loop); // 执行构造 EventLoopThread 时的 cb
  }

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();		// 通知主线程创建完成
  }

  loop.loop();			// 启动事件循环后阻塞在当前线程，直到.quit()
  //assert(exiting_);
  MutexLockGuard lock(mutex_);
  loop_ = NULL;
}

