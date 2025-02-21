# Chat Server and Client

A simple chat server and client application built with C using sockets and pthreads. This project allows two clients to connect to the server and exchange messages in real-time.

## Features

- **Server**: Accepts two client connections, relays messages between them.
- **Client**: Sends and receives messages to/from the server.
- **Multithreading**: Each client runs in a separate thread to handle message sending and receiving concurrently.
- **Socket Communication**: Uses TCP sockets for reliable communication between server and client.

## Prerequisites

- **GCC** or any C compiler that supports C99 or later.
- **pthread** library for multithreading.

## Compilation and Running

### Server

1. Navigate to the Server directory.
2. Compile the server using `make`:
   ```bash
   make
Run the server:
./server

### Client

1. Navigate to the Client directory.
2. Compile the client using `make`:
   ```bash
   make
Run the client:
./client

Running Multiple Clients
To test the chat functionality:

Start the server first.
Start two client instances by running the client application twice in separate terminals:
./client
Both clients will be able to send and receive messages from each other through the server.

File Structure
/server
    server.c
    server.h
    main.c
    Makefile
/client
    client.c
    client.h
    main.c
    Makefile
README.md
Notes
The server is designed to handle exactly two clients at a time. To extend this, modify the code to support more clients (e.g., using a dynamic list of sockets and more complex thread management).
If a client disconnects, the other client will be notified, and the server will close the connection.