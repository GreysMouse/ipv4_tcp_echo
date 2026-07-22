#ifndef _COMPAT_H
#define _COMPAT_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCK_INV -1
#define SOCK_ERR -1

typedef int sock_t;
typedef int sockopt_t;

static inline void num_perror(const char *s, int errnum)
{
    fprintf(stderr, "%s: %s\n", s, strerror(errnum));
}

static inline void sock_perror(const char *s) { perror(s); }

static inline int sock_setup(void) { return 0; }

static inline int sock_destroy(void) { return 0; }

static inline int sock_close(sock_t fd) { return close(fd); }

#else

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SOCK_INV INVALID_SOCKET
#define SOCK_ERR SOCKET_ERROR

typedef SOCKET sock_t;
typedef char sockopt_t;
typedef uint16_t in_port_t;

void num_perror(const char *s, int errnum);

static inline void sock_perror(const char *s)
{
    num_perror(s, WSAGetLastError());
}

static inline int sock_setup(void)
{
    WSADATA data;

    return WSAStartup(MAKEWORD(2, 2), &data);
}

static inline int sock_destroy(void) { return WSACleanup(); }

static inline int sock_close(sock_t fd) { return closesocket(fd); }

#endif

#endif