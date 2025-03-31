// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include "muduo/net/Buffer.h"

#include "muduo/net/SocketsOps.h"
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>         // 供writeFd()实现


using namespace muduo;
using namespace muduo::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  char extrabuf[65536];			//（1）额外栈上内存
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;   // iovec[0]指向 Buffer
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;               // iovec[1]指向 extrabuf
  vec[1].iov_len = sizeof extrabuf;
  //（2）当 writable 不足时再使用栈上内存 extrabuf
  const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
  //（3）readv 散读特性读取数据
  const ssize_t n = sockets::readv(fd, vec, iovcnt);

  if (n < 0)
  {
    *savedErrno = errno;
  }
  else if (implicit_cast<size_t>(n) <= writable)
  {
    writerIndex_ += n;				//（4）读入的数据不多时，全部读到 Buffer
  }
  else
  {
    writerIndex_ = buffer_.size();	//（5）数据超过 Buffer 容量，先读到栈上内存 extrabuf
    append(extrabuf, n - writable); // 然后 Buffer 腾挪/扩容后，再 copy 栈内存数据 extrabuf
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}


ssize_t Buffer::writeFd(int fd, int* saveErrno) {
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0) {
        *saveErrno = errno;
    }
    return n;
}

