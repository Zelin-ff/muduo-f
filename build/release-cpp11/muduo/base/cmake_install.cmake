# Install script for directory: /mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/mnt/hgfs/Demo01Share/muduo-f/build/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/lib/libmuduo_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/AsyncLogging.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Atomic.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/BlockingQueue.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/BoundedBlockingQueue.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Condition.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/CountDownLatch.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/CurrentThread.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Date.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Exception.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/FileUtil.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/GzipFile.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/LogFile.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/LogStream.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Logging.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Mutex.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/ProcessInfo.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Singleton.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/StringPiece.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Thread.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/ThreadLocal.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/ThreadLocalSingleton.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/ThreadPool.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/TimeZone.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Timestamp.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/Types.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/WeakCallback.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/copyable.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/muduo/base/tests/cmake_install.cmake")

endif()

