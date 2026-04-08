#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080
#define MAX_EVENTS 10
#define MAX_CLIENTS 1024
#define CLIENT_BUF 4096

typedef struct {
    int fd;
    char buf[CLIENT_BUF];
    int buf_len;
} Client;

static Client clients[MAX_CLIENTS];

static Client *client_new(int fd)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].fd == -1) {
            clients[i].fd = fd;
            clients[i].buf_len = 0;
            memset(clients[i].buf, 0, CLIENT_BUF);
            return &clients[i];
        }
    }
    return NULL;
}

static Client *client_find(int fd)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].fd == fd)
            return &clients[i];
    }
    return NULL;
}

static void client_free(Client *c)
{
    close(c->fd);
    c->fd = -1;
    c->buf_len = 0;
}

void set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) { perror("fcntl F_GETFL"); return; }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        perror("fcntl F_SETFL");
}

int setup_server(int port)
{
    int master_socket;
    struct sockaddr_in address;
    int opt = 1;

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed"); exit(EXIT_FAILURE);
    }
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed"); exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed"); exit(EXIT_FAILURE);
    }
    if (listen(master_socket, 10) < 0) {
        perror("Listen failed"); exit(EXIT_FAILURE);
    }

    set_nonblocking(master_socket);
    return master_socket;
}

int setup_epoll(int master_socket)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) { perror("epoll_create1 failed"); exit(EXIT_FAILURE); }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = master_socket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, master_socket, &event) == -1) {
        perror("epoll_ctl: master_socket"); exit(EXIT_FAILURE);
    }
    return epoll_fd;
}

void accept_new_connection(int epoll_fd, int master_socket)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int new_socket = accept(master_socket, (struct sockaddr *)&client_addr, &client_len);
    if (new_socket == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept failed");
        return;
    }

    printf("New connection! Socket fd: %d, IP: %s\n",
           new_socket, inet_ntoa(client_addr.sin_addr));

    set_nonblocking(new_socket);

    Client *c = client_new(new_socket);
    if (!c) {
        fprintf(stderr, "Client table full – dropping fd %d\n", new_socket);
        close(new_socket);
        return;
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = new_socket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
        perror("epoll_ctl: new_socket");
        client_free(c);
    }
}

void handle_client_data(int client_socket)
{
    Client *c = client_find(client_socket);
    if (!c) {
        fprintf(stderr, "Unknown fd %d – closing\n", client_socket);
        close(client_socket);
        return;
    }

    while (1) {
        int space = CLIENT_BUF - c->buf_len - 1;
        if (space <= 0) {
            fprintf(stderr, "Buffer overflow on fd %d – closing\n", c->fd);
            client_free(c);
            return;
        }

        int n = read(c->fd, c->buf + c->buf_len, space);

        if (n == 0) {
            printf("Client disconnected! Socket fd: %d\n", c->fd);
            client_free(c);
            return;
        }

        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            perror("read error");
            client_free(c);
            return;
        }

        c->buf_len += n;
    }

    if (c->buf_len > 0) {
        c->buf[c->buf_len] = '\0';
        printf("Complete message from fd %d (%d bytes): %s",
               c->fd, c->buf_len, c->buf);

        send(c->fd, c->buf, c->buf_len, 0);

        c->buf_len = 0;
        memset(c->buf, 0, CLIENT_BUF);
    }
}

int main(void)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i].fd = -1;

    int master_socket = setup_server(PORT);
    int epoll_fd = setup_epoll(master_socket);

    struct epoll_event events[MAX_EVENTS];
    printf("epoll server is running on port %d...\n", PORT);

    for (;;) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) { perror("epoll_wait failed"); exit(EXIT_FAILURE); }

        for (int n = 0; n < nfds; n++) {
            if (events[n].data.fd == master_socket)
                accept_new_connection(epoll_fd, master_socket);
            else
                handle_client_data(events[n].data.fd);
        }
    }

    close(master_socket);
    close(epoll_fd);
    return 0;
}