#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

/* Writes len bytes to fd. Returns number of written chars.
 * Message in buf must be null-terminated.
 */
static size_t push_msg(int fd, const char *buf, size_t len)
{
    size_t r;
    ssize_t n;

    r = len;

    while (r) {
        n = write(fd, buf, r);

        if (n <= 0 && errno != EINTR) {
            break;
        }
        buf += n;
        r -= n;
    }
    return len - r;
}

/* Reads null-terminated message into buf of size len.
 * Returns the number of bytes read (null-terminator included).
 */
static size_t pull_msg(int fd, char *buf, size_t len)
{
    size_t acc;
    ssize_t n, i;

    for (acc = 0; acc < len; acc += n) {
        n = read(fd, buf + acc, len - acc);

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