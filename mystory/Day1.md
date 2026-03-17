---
title: Day 1
date: 2026-03-17
topic: TCP Server Boilerplate in C
---

# Day 1 — TCP Server Boilerplate in C

Today I took my first official step into raw, system‑level C network programming. I wrote the boilerplate to initialize a TCP server that speaks directly to the Linux OS: it allocates a network socket, clears memory, handles endianness translation, binds to a specific port, and enters a passive listening state.

---

## The Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8030
#define SA struct sockaddr

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // 1. Socket Creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket Successfully created..\n");
    }

    // 2. Memory Sanitization
    bzero(&servaddr, sizeof(servaddr));

    // 3. Network Configuration & Endianness Translation
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // 4. Binding to the Operating System
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    } else {
        printf("Socket successfully binded...\n");
    }

    // 5. Entering the Listening State
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening..\n");
    }

    len = sizeof(cli);
    return 0;
}
```

---

## Deep Dive Notes

1. **The Socket File Descriptor (`socket()`)**  
   In Linux, everything is treated as a file. Calling `socket()` tells the OS kernel to allocate a new communication endpoint and return an integer index known as a **File Descriptor**. `AF_INET` specifies the IPv4 protocol, and `SOCK_STREAM` specifies a reliable, two‑way TCP stream.

1. **Memory Sanitization (`bzero()`)**  
   When a struct is declared in C, it grabs raw memory that might contain leftover garbage data from previous processes. `bzero()` wipes that memory block with zeroes. Skipping this step can lead to corrupted IP addresses or ports when the program runs.

1. **Endianness & Hardware Translation (`htonl()` / `htons()`)**  
   Different CPUs map memory in reverse orders. Intel machines natively use Little‑Endian, while the internet infrastructure universally demands Big‑Endian (Network Byte Order).
   - `htonl()` (Host to Network Long) translates the 32‑bit IP address into the network standard.
   - `htons()` (Host to Network Short) does the same for the 16‑bit port number.

1. **The Struct Casting Trick (`bind()`)**  
   The `bind()` function links our application to a specific OS port. It strictly requires a generic `struct sockaddr`. Because filling a generic array by hand is difficult, we use the perfectly divided `struct sockaddr_in` to set IP and port, then cast it as `(SA*)&servaddr` at the last second to satisfy the system requirement.

1. **The Connection Queue (`listen()`)**  
   This transitions the socket into a passive state. The `5` represents the backlog — it tells the kernel to queue up to five incoming client connection requests before it starts rejecting new ones.
