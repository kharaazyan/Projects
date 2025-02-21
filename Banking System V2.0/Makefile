# --------------------------------------------------
#  Makefile for Banking System V2.0 without bin dir
# --------------------------------------------------

# Компилятор и флаги
CXX       = g++
CXXFLAGS  = -Wall -Wextra -O2 -std=c++20
INCLUDES  = -I./include

# Итоговый исполняемый файл (в текущем каталоге)
TARGET    = BankingApp

# Список .cpp файлов
SRCS = \
  src/Account.cpp \
  src/LogService.cpp \
  src/BankApplication.cpp \
  src/CubeEncoder.cpp \
  src/DatabaseData.cpp \
  src/DatabaseMeta.cpp \
  src/EnumClassStatus.cpp \
  src/Login.cpp \
  src/Main.cpp \
  src/Registration.cpp \
  src/User.cpp \
  src/Utility.cpp

# Правило для получения .o из .cpp
OBJS = $(SRCS:.cpp=.o)

# -- Правило сборки (линковки):
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# -- Правило для .o файлов:
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# -- Цель 'clean':
clean:
	rm -f $(OBJS)

# -- Цель 'run' для быстрого запуска:
run: $(TARGET)
	./$(TARGET)

# -- Цель 'rebuild':
rebuild: clean $(TARGET)

.PHONY: clean run rebuild
