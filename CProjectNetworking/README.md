# Network Communication Module

This repository contains C code for implementing a simple network communication module using sockets and pthreads. The codebase consists of server-side and client-side implementations for establishing a connection and transferring data securely.

## Table of Contents

- [Introduction](#introduction)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Compile](#compile-with)
## Introduction

The `network.h` and `network.c` files provide functions and structures to create a server and client, handle connections, send and receive data securely over a network using sockets, and manage multithreading for multiple client connections.

## Usage

### Server
- To run the server executable, navigate to the `server` directory and execute the `serverExecutable`.
- The `server.c` file initializes a server, listens for incoming connections, and creates a thread for each connected client. 
- It uses pthreads to handle multiple client connections concurrently.

### Client
- The `client` directory contains the `client.c` file.
- Compile and execute the `client.c` file to establish a connection to the server and exchange data securely.
- It uses functions defined in `network.h` and `network.c` to send and receive data to/from the server.

## File Structure

├── c_core
│ ├── include
│ │ └── network.h
│ └── src
│ └── network.c
├── client
│ └── client.c
├── server
│ └── server.c
└── README.md

- `c_core` directory contains the source code for the network communication module.
- `client` directory holds the client-side code.
- `server` directory contains the server-side code.
- `clientExecutable` and `serverExecutable` directories store compiled executable files for respective client and server implementations.

## Note

This codebase provides a foundational structure for network communication in C, demonstrating basic client-server interactions, multithreading, and secure data exchange using sockets and pthreads. Feel free to extend and modify this code according to your requirements.

## Compile with
gcc server/server.c c_core/src/network.c -o serverExecutable -lpthread -lssl -lcrypto
gcc client/client.c c_core/src/network.c -o clientExecutable -lssl -lcrypto

