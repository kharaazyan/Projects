CC = gcc
CFLAGS = -Wall -pthread
TARGET = server

all: $(TARGET)

$(TARGET): main.c server.c
	$(CC) $(CFLAGS) main.c server.c -o $(TARGET)

clean:
	rm -f $(TARGET)
