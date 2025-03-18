# 指定编译器
CXX = g++

# 编译选项
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# 需要链接的库
LDFLAGS = -lmuduo_net -lmuduo_base -lpthread

# 目标文件
TARGET = echo

# 源文件
SRCS = echo.cc

# 生成的对象文件
OBJS = $(SRCS:.cc=.o)

# 规则：默认目标
all: $(TARGET)

# 规则：编译可执行文件
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# 规则：编译源文件
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理编译文件
clean:
	rm -f $(OBJS) $(TARGET)
