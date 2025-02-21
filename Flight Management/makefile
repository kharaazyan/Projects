CC = gcc
CFLAGS = -Wall -Wextra -Icommon/include

SERVER_SRC = server/src
SERVER_INC = server/include
CLIENT_SRC = client/src
CLIENT_INC = client/include
COMMON_INC = common/include
COMMON_SRC = common/src

SERVER_OBJS = \
    $(SERVER_SRC)/main.o \
    $(SERVER_SRC)/server.o \
    $(SERVER_SRC)/admin.o \
    $(SERVER_SRC)/flight_management.o

CLIENT_OBJS = \
    $(CLIENT_SRC)/main.o \
    $(CLIENT_SRC)/client.o

COMMON_OBJS = $(COMMON_SRC)/common.o

SERVER_BIN = server_app
CLIENT_BIN = client_app

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -I$(SERVER_INC) $^ -lpthread -o $@

$(CLIENT_BIN): $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -I$(CLIENT_INC) $^ -o $@

$(SERVER_SRC)/%.o: $(SERVER_SRC)/%.c
	$(CC) $(CFLAGS) -I$(SERVER_INC) -c $< -o $@

$(CLIENT_SRC)/%.o: $(CLIENT_SRC)/%.c
	$(CC) $(CFLAGS) -I$(CLIENT_INC) -c $< -o $@

$(COMMON_SRC)/%.o: $(COMMON_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_SRC)/*.o
	rm -f $(CLIENT_SRC)/*.o
	rm -f $(COMMON_SRC)/*.o
	rm -f $(SERVER_BIN)
	rm -f $(CLIENT_BIN)

.PHONY: all clean
