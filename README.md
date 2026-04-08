# Unix Networking Study Journal

Hands-on Linux/Unix network programming notes and C practice files.  
The goal is to move from basic socket programming to scalable event-driven servers.

## Learning Focus

- Build TCP/UDP programs directly against the socket API
- Understand byte order, address structures, and connection lifecycle
- Practice blocking and non-blocking I/O patterns
- Move from `select()` to `epoll`-based server design

## Repository Layout

- `TCP/` - Basic TCP examples
- `UDP/` - Basic UDP examples
- `TcpDaytimeSystem/` - Daytime client/server practice
- `un-blocking/` - Non-blocking server with `select()`
- `epoll_socket_server/` - Event-driven server experiments with `epoll`
- `http_server.c` - Non-blocking `epoll` HTTP static file server (single-file implementation)
- `study-logs/` - Daily learning notes

## Study Progress

| Day | Topic | Note |
|---|---|---|
| 1 | TCP server boilerplate in C | [Day1.md](study-logs/Day1.md) |
| 2 | TCP client/server communication basics | [Day2.md](study-logs/Day2.md) |
| 3 | OSI model and sockets interface | [Day3.md](study-logs/Day3.md) |
| 4 | UDP client/server fundamentals | [Day4.md](study-logs/Day4.md) |
| 5 | Non-blocking TCP server with `select()` | [Day5.md](study-logs/Day5.md) |
| 6 | Building `Tcp_epoll_server.c` from scratch | [Day6.md](study-logs/Day6.md) |
| 7 | Structuring and polishing the `epoll` server workflow | [Day7.md](study-logs/Day7.md) |
| 8 | Building an `epoll`-based HTTP static file server | [Day8.md](study-logs/Day8.md) |

## Core Topics Covered

- `socket()`, `bind()`, `listen()`, `accept()`
- `recvfrom()` and `sendto()` for UDP
- IPv4 addressing and socket structures
- Non-blocking sockets with `fcntl()`
- Multiplexing with `select()`
- Event-driven design with `epoll`
- Basic HTTP request parsing and static file responses

## References

- Beej's Guide to Network Programming
- UNIX Network Programming: The Sockets Networking API
- Linux manual pages (`man 2 socket`, `man 7 epoll`, etc.)

This repository is a revision-friendly learning trail: each day records what was built, what was understood, and what comes next.
