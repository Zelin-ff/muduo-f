# Install script for directory: /mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/lib/libmuduo_net.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/net" TYPE FILE FILES
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/Buffer.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/Callbacks.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/Channel.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/Endian.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/EventLoop.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/EventLoopThread.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/EventLoopThreadPool.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/InetAddress.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/TcpClient.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/TcpConnection.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/TcpServer.h"
    "/mnt/hgfs/Demo01Share/muduo-f/muduo_src/muduo/net/TimerId.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/muduo/net/http/cmake_install.cmake")
  include("/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/muduo/net/inspect/cmake_install.cmake")
  include("/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/muduo/net/tests/cmake_install.cmake")
  include("/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/muduo/net/protobuf/cmake_install.cmake")
  include("/mnt/hgfs/Demo01Share/muduo-f/build/release-cpp11/muduo/net/protorpc/cmake_install.cmake")

endif()

