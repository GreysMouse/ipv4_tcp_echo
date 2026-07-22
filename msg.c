#include "msg.h"

size_t push_msg(sock_t fd, const char *buf, size_t len)
{
    size_t r;
    ssize_t n;

    r = len;

    while (r) {
        n = send(fd, buf, r, 0);

        if (n <= 0 && errno != EINTR) {
            break;
        }
        buf += n;
        r -= n;
    }
    return len - r;
}

size_t pull_msg(sock_t fd, char *buf, size_t len)
{
    size_t acc;
    ssize_t n, i;

    for (acc = 0; acc < len; acc += n) {
        n = recv(fd, buf + acc, len - acc, 0);

        if (n <= 0 && errno != EINTR) {
            break;
        }

        for (i = n - 1; i >= 0; i--) {
            if (buf[acc + i] == '\0') {
                return acc + i + 1;
            }
        }
    }
    return acc;
}