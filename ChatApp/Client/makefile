CC = gcc
CFLAGS = -Wall -pthread
TARGET = client

all: $(TARGET)

$(TARGET): main.c client.c
	$(CC) $(CFLAGS) main.c client.c -o $(TARGET)

clean:
	rm -f $(TARGET)
