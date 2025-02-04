# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -g -Wall

# 目标文件和源文件
SRCS = main.cpp Server.cpp Logger/Logger.cpp
ASSEMBLY = $(SRCS:.cpp=.s)
OBJS = $(ASSEMBLY:.s=.o)

# 最终生成的可执行文件
TARGET = server

# 默认目标，生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

# 编译 .cpp 为汇编文件
%.s: %.cpp
	$(CXX) -S $< $(CXXFLAGS)

# 编译 .s 为 .o 文件
%.o: %.s
	$(CXX) -c $< $(CXXFLAGS)

# 清理
clean:
	rm -f $(OBJS) $(ASSEMBLY) $(TARGET)

# 伪目标
.PHONY: clean
