#ifndef FILE_SYS_H
#define FILE_SYS_H

#include "syscalls.h"
#include "io_sys.h"

#define SYS_O_RDONLY 0
#define SYS_O_WRONLY 1
#define SYS_O_RDWR   2
#define SYS_O_CREAT  0100
#define SYS_O_TRUNC  01000
#define SYS_MODE_644 0644

static inline int sys_open_file(const char* path, int flags, int mode) {
    return (int)sys_call(SYS_OPEN, (long)path, flags, mode);
}

static inline void sys_close_file(int fd) {
    sys_call(SYS_CLOSE, fd, 0, 0);
}

static inline long sys_read_file(int fd, char* buf, long n) {
    return sys_call(SYS_READ, fd, (long)buf, n);
}

static inline long sys_write_file(int fd, const char* buf, long n) {
    return sys_call(SYS_WRITE, fd, (long)buf, n);
}

#endif // FILE_SYS_H
