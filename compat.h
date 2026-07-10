#ifndef _COMPAT_H
#define _COMPAT_H

#include <stdint.h>
#include <stdio.h>

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

static int sock_setup(void) { return 0; }

static int sock_close(sock_t fd) { return close(fd); }

static void sock_perror(const char *s) { perror(s); }

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

static int sock_setup(void)
{
    WSADATA data;

    return WSAStartup(MAKEWORD(2, 2), &data);
}

static int sock_close(sock_t fd) { return closesocket(fd); }

static void sock_perror(const char *s)
{
    int err;
    LPSTR msg;
    DWORD len;

    err = WSAGetLastError();
    len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                            FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL, (DWORD)err, 0, (LPSTR)&msg, 0, NULL);

    if (len == 0) {
        fprintf(stderr, "%s: error code %d\n", s, err);
    } else {
        fprintf(stderr, "%s: %s\n", s, msg);
        LocalFree(msg);
    }
}

#endif

#endif