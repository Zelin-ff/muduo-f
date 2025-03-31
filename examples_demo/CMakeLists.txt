cmake_minimum_required(VERSION 3.10)
project(echo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_VERBOSE_MAKEFILE ON)          # 是否输出Makefile的详细信息

set (LIBS
    muduo_net
    muduo_base
    pthread
)

# 2️ 生成可执行文件并自动链接
function(self_add_executable targetname srcs)
    add_executable(${targetname} ${srcs})
    target_link_libraries(${targetname} ${LIBS})
endfunction()

self_add_executable(netty_echoServer netty/echo/echoServer.cc)
self_add_executable(netty_echoClient netty/echo/echoClient.cc)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)