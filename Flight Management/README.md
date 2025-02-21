# README — Flight Management Project

This project demonstrates a basic **client-server** application in C for managing flight information. It includes the following features:

1. **Server-Side**:
   - **Administrator menu** for managing flights (create, list, delete) and for changing the ticket price.
   - **Flight booking** functionality (clients can reserve seats).
   - **Multi-threaded** handling of client connections (each client runs in a separate thread).
   - **Synchronization** with a pthread **mutex** to protect shared data structures.

2. **Client-Side**:
   - Displays available flights (received from the server).
   - Allows booking a flight by sending the appropriate command to the server.
   - Cleanly closes the connection when exiting.

3. **Common** code includes shared data structures, constants, and type definitions.

---

## Project Structure

A suggested directory layout could be:

```
flight_management_project
├── server
│   ├── include
│   │   ├── admin.h
│   │   ├── flight_management.h
│   │   └── server.h
│   └── src
│       ├── admin.c
│       ├── flight_management.c
│       ├── server.c
│       └── main.c
├── client
│   ├── include
│   │   └── client.h
│   └── src
│       ├── client.c
│       └── main.c
├── common
│   ├── include
│   │   └── common.h
│   └── src
│       └── common.c
└── Makefile
```

### Explanation of Folders:

- **`server/`**  
  Contains all server-side code:
  - `main.c`: Entry point where the server initialization and accept-loop runs.
  - `server.c` & `server.h`: Main server routines (socket creation, listening, client-thread handler).
  - `admin.c` & `admin.h`: Administrator-related functionality (login/password, menus, ticket-price modification).
  - `flight_management.c` & `flight_management.h`: Logic for creating, deleting, listing flights, and booking.

- **`client/`**  
  Contains all client-side code:
  - `main.c`: Entry point for the client application (presents a menu, connects to the server).
  - `client.c` & `client.h`: Client networking logic (show flights, book flights, handle server responses).

- **`common/`**  
  Houses common definitions shared between the server and the client:
  - `common.h`: Data structures (e.g., `Flight`, `FlightNode`, etc.), constants (port, buffer sizes).
  - `common.c`: (Optional) if you have shared functions used by both client and server.

- **`Makefile`**  
  A single `Makefile` at the project root (or separate makefiles in each sub-folder). It compiles both server and client binaries.

---

## Building the Project

1. **Clone or download** the project into a folder, e.g. `flight_management_project/`.
2. Ensure you have a **C compiler** (`gcc` or `clang`) and **pthread library** installed.
3. Inside the project root, run:
   ```bash
   make
   ```
   This will produce two executables (if the Makefile is set up as shown):
   - `server_app`
   - `client_app`

If you prefer separate compilation, adapt the commands and file paths in `Makefile` accordingly.

---

## Running the Project

1. **Start the server**:
   ```bash
   ./server_app
   ```
   - The server will prompt for **administrator nickname** and **password**.  
   - By default (in the sample code) they are `"kharaazyan\n"` and `"hovik2003\n"` (including newline because of `fgets`).  
   - After successful login, the admin menu will appear, allowing you to:
     - Show available flights
     - Create a new flight
     - Delete a flight
     - Change the ticket price
     - End the server

2. **Run the client** (in a separate terminal):
   ```bash
   ./client_app
   ```
   - The client will connect to the server and display a menu with options:
     - Show available flights
     - Book a flight
     - Exit

3. **Flight Booking**:
   - When booking a flight, the client will send a request to the server and prompt for flight ID, user name, and contact info.  
   - The server decreases the available seats for that flight and stores a record in a linked list of clients for that flight.

4. **Exiting**:
   - From the client side, choose **Exit** in the menu. The client sends the “exit” command to the server and closes the connection.
   - From the server side (admin menu), if you choose **End the server work**, the application will terminate all threads, close the socket, and exit.

---


## Potential Extensions

- **Persistent storage**: Save flight and booking info to a file or database.
- **Authentication** of regular clients.
- **Search/filter** flights by source, destination, or availability.
- **Cancellation** of bookings or modifying existing bookings.