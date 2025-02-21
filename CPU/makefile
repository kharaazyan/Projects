# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Директории
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Целевое имя бинарника
TARGET := $(BIN_DIR)/cpu_sim

# Исходные файлы
SRCS := $(SRC_DIR)/main.cpp \
        $(SRC_DIR)/cpu.cpp \
        $(SRC_DIR)/utils.cpp

# Объектные файлы (obj)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Правило сборки
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Дополнительные цели
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Фоновые (нефайловые) цели
.PHONY: all clean
