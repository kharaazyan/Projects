CXX         = g++
CXXFLAGS    = -std=c++20 -I./tgbot-cpp/include -Wall
LDLIBS      = -lboost_system -lboost_filesystem -lcurl -lssl -lcrypto -lpthread

# Путь к статической библиотеке TgBot
LIB_TGBOT   = include/tgbot-cpp/build/libTgBot.a

# Исходные файлы проекта
PROJECT_SRCS  = main.cpp 
PROJECT_OBJS  = $(PROJECT_SRCS:.cpp=.o)

TARGET      = bot

all: $(TARGET)
	@clear
	@echo "Сборка завершена!"

# Линковка итогового бинарника с использованием объекта проекта и статической библиотеки TgBot
$(TARGET): $(PROJECT_OBJS)
	$(CXX) $(PROJECT_OBJS) $(LIB_TGBOT) -o $(TARGET) $(LDLIBS)

# Общее правило компиляции .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка объектных файлов и бинарника
clean:
	rm -f $(PROJECT_OBJS) $(TARGET)
	@clear

# Полная пересборка: сначала clean, затем all
rebuild: clean all

# Установка зависимостей (Linux / macOS)
install_deps:
	@echo "Определение операционной системы..."
	@if [ "`uname`" = "Linux" ]; then \
		echo "Установка зависимостей для Linux..."; \
		sudo apt-get update && sudo apt-get install -y \
		libboost-all-dev \
		libcurl4-openssl-dev \
		libssl-dev \
		build-essential; \
	elif [ "`uname`" = "Darwin" ]; then \
		echo "Установка зависимостей для macOS..."; \
		brew update && brew install \
		boost \
		openssl \
		curl; \
	else \
		echo "ОС не поддерживается. Установите зависимости вручную."; \
		exit 1; \
	fi
