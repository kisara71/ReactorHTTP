# === 编译器 & 标志 ===
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2 -g
INCLUDES := -Iinclude

# === 目录结构 ===
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/server

# === 查找所有源文件（递归）===
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# === 默认目标 ===
all: $(TARGET)

# === 编译链接 ===
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === 编译规则 ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# === 清理 ===
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
