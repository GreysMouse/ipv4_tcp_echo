#include "compat.h"
#include "config.h"
#include "msg.h"
#include <stdlib.h>
#include <string.h>

static sock_t create_listen_socket(void)
{
    sock_t fd;
    sockopt_t opt;
    struct sockaddr_in addr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == SOCK_INV) {
        return SOCK_INV;
    };

    opt = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ==
        SOCK_ERR) {
        return SOCK_INV;
    }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == SOCK_ERR) {
        return SOCK_INV;
    }

    if (listen(fd, LISTENQ) == SOCK_ERR) {
        return SOCK_INV;
    };

    return fd;
}

static void handle_connection(sock_t fd, const struct sockaddr_in *addr)
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
    sock_t s_fd;

    if (sock_setup() != 0) {
        fprintf(stderr, "[sock_setup]: error");
        exit(EXIT_FAILURE);
    }

    if ((s_fd = create_listen_socket()) == SOCK_INV) {
        sock_perror("Listen socket");
        exit(EXIT_FAILURE);
    };

    // for (;;) {
    //     sock_t c_fd;
    //     struct sockaddr_in c_addr;
    //     socklen_t c_addr_len;
    //     pid_t p_id;

    //     c_addr_len = sizeof(c_addr);
    //     c_fd = accept(s_fd, (struct sockaddr *)&c_addr, &c_addr_len);

    //     if (c_fd == SOCK_INV) {
    //         sock_perror("Connected socket");
    //         continue;
    //     };

    //     if ((p_id = fork()) == 0) {
    //         sock_close(s_fd);
    //         handle_connection(c_fd, &c_addr);
    //         sock_close(c_fd);
    //         exit(EXIT_SUCCESS);
    //     };

    //     sock_close(c_fd);
    // };

    return EXIT_SUCCESS;
}