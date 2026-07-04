#include "config.h"
#include "msg.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int create_listen_socket(void)
{
    int fd, opt;
    struct sockaddr_in addr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    };

    opt = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        return -1;
    }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
        return -1;
    }

    if (listen(fd, LISTENQ) == -1) {
        return -1;
    };

    return fd;
}

static void handle_connection(int fd, const struct sockaddr_in *addr)
{
    in_port_t port;
    size_t msg_len;

    char ip[INET_ADDRSTRLEN];
    char greet[INET_ADDRSTRLEN + 16];
    char msg[256];

    port = ntohs(addr->sin_port);
    inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);

    snprintf(greet, sizeof(greet), "Welcome, %s:%d", ip, port);
    push_msg(fd, greet, strlen(greet) + 1); /* null-terminated */

    printf("%s:%d connected\n", ip, port);

    for (;;) {
        msg_len = pull_msg(fd, msg, sizeof(msg));

        if (!msg_len) /* if connection closed by them */ {
            printf("%s:%d disconnected\n", ip, port);
            break;
        } else {
            push_msg(fd, msg, msg_len);
        }
    }
}

int main(void)
{
    int s_fd;

    if ((s_fd = create_listen_socket()) == -1) {
        perror("Listen socket");
        exit(EXIT_FAILURE);
    };

    for (;;) {
        int c_fd;
        struct sockaddr_in c_addr;
        socklen_t c_addr_len;
        pid_t p_id;

        c_addr_len = sizeof(c_addr);
        c_fd = accept(s_fd, (struct sockaddr *)&c_addr, &c_addr_len);

        if (c_fd == -1) {
            perror("Connected socket");
            continue;
        };

        if ((p_id = fork()) == 0) {
            close(s_fd);
            handle_connection(c_fd, &c_addr);
            close(c_fd);
            exit(EXIT_SUCCESS);
        };

        close(c_fd);
    };

    return EXIT_SUCCESS;
}