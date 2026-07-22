#ifndef _MSG_H
#define _MSG_H

#include "compat.h"

/* Writes len bytes to fd. Returns number of written chars.
 * Message in buf must be null-terminated.
 */
size_t push_msg(sock_t fd, const char *buf, size_t len);

/* Reads null-terminated message into buf of size len.
 * Returns the number of bytes read (null-terminator included).
 */
size_t pull_msg(sock_t fd, char *buf, size_t len);

#endif