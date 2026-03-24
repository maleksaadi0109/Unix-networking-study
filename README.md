# Unix Networking - Study Log

This repository is my learning journal for low-level Unix and Linux network programming in C.

Current milestone: I moved from basic TCP and UDP examples into a non-blocking TCP server using `select()` and file descriptor sets.

## What This Repo Is For

- Study core networking ideas like sockets, TCP, UDP, byte order, blocking vs non-blocking I/O, and how the OS handles connections.
- Build small C programs that talk directly to the kernel networking stack.
- Keep daily study notes that are easy to review later.

## Study Log Structure

Each day is stored inside `study-logs/`:

- `Day1.md`
- `Day2.md`
- `Day3.md`
- `Day4.md`
- `Day5.md`

Each note includes:

- What I studied
- The main code pattern
- Important concepts
- A short reflection

## Daily Notes

- [Day 1](study-logs/Day1.md) - TCP server boilerplate in C
- [Day 2](study-logs/Day2.md) - TCP client/server communication basics
- [Day 3](study-logs/Day3.md) - OSI model, sockets interface, and the TCP daytime server
- [Day 4](study-logs/Day4.md) - UDP client/server basics and finishing Chapter 1
- [Day 5](study-logs/Day5.md) - Non-blocking TCP server with `select()`

## Topics Covered So Far

- TCP socket setup with `socket()`, `bind()`, `listen()`, and `accept()`
- Iterative TCP servers
- UDP communication with `recvfrom()` and `sendto()`
- IPv4 basics
- Socket API structure inside the networking stack
- Non-blocking sockets with `fcntl()`
- I/O multiplexing with `select()`

## References I Use

- Beej's Guide to Network Programming
- UNIX Network Programming: The Sockets Networking API
- GeeksforGeeks

This repo is meant to show progress day by day, so the notes are written in a way that helps me revise later, not just record code.


