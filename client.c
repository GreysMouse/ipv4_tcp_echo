#include "config.h"
#include "msg.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    struct sockaddr_in addr;
    char msg[256];
    size_t msg_len;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    };

    if ((connect(fd, (const struct sockaddr *)&addr, sizeof(addr))) == -1) {
        perror("Connect");
        exit(EXIT_FAILURE);
    }

    msg_len = pull_msg(fd, msg, sizeof(msg));

    if (msg_len) {
        fprintf(stdout, "%s\n", msg);
    }

    for (;;) {
        if (fgets(msg, sizeof(msg), stdin) == NULL) {
            fprintf(stderr, "Nothing was read from stdin\n");
            break;
        };

        push_msg(fd, msg, strlen(msg) + 1);
        pull_msg(fd, msg, sizeof(msg));

        printf("%s\n", msg);
    }
    close(fd);

    return EXIT_SUCCESS;
}