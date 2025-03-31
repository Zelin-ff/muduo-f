cmake_minimum_required(VERSION 3.10)
project(echo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1（1）指定 Muduo 头文件路径，库文件路径
include_directories(/usr/include)
link_directories(/usr/lib)

# （2）使用 pkg-config 自动查找 Muduo
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(MUDUO REQUIRED muduo)


set (LIBS
    muduo_net
    muduo_base
    pthread
)

# 2️ 生成可执行文件
add_executable(server echoServer.cc)
add_executable(client echoClient.cc)

# 3️ 链接 Muduo 库
target_include_directories(server PRIVATE ${MUDUO_INCLUDE_DIRS})
target_link_libraries(server ${LIBS})

target_include_directories(client PRIVATE ${MUDUO_INCLUDE_DIRS})
target_link_libraries(client ${LIBS})


set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
