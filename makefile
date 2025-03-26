CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread -g

# 文件夹路径
OBJ_DIR = obj
BIN_DIR = bin
SRC_DIR = src

# 源文件
SRCS = $(wildcard Channel/*.cpp) \
       $(wildcard Config/*.cpp) \
       $(wildcard Dispatcher/EpollDispatcher/*.cpp) \
       $(wildcard Dispatcher/PollDispatcher/*.cpp) \
       $(wildcard Dispatcher/SelectDispatcher/*.cpp) \
       $(wildcard EventLoop/*.cpp) \
       $(wildcard HTTPRequest/*.cpp) \
       $(wildcard Logger/*.cpp) \
       $(wildcard Server/*.cpp) \
       $(wildcard ThreadPool/*.cpp) \
       main.cpp \

# 目标文件
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# 可执行文件
TARGET = $(BIN_DIR)/server

# 默认目标
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)  # 确保 bin 文件夹存在
	$(CC) $(CXXFLAGS) -o $@ $^

# 编译源文件到目标文件
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

# 清理目标
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
