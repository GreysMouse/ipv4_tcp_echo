#include "compat.h"

#ifdef _WIN32

void num_perror(const char *s, int errnum)
{
    LPSTR msg;
    DWORD len;

    len = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, (DWORD)errnum, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPSTR)&msg, 0, NULL);

    if (len == 0) {
        fprintf(stderr, "%s: error code %d\n", s, errnum);
    } else {
        fprintf(stderr, "%s: %s", s, msg);
        LocalFree(msg);
    }
}

#endif