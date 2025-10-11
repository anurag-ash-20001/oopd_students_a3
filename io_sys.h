#ifndef IO_SYS_H
#define IO_SYS_H

#include "syscalls.h"

// write buffer to fd (usually stdout=1)
static inline long sys_write_fd(int fd, const char* buf, long len) {
    return sys_call(SYS_WRITE, fd, (long)buf, len);
}

// write C-string to stdout
static inline void sys_write_str(const char* s) {
    long len = 0;
    while (s[len]) ++len;
    sys_write_fd(1, s, len);
}

// write newline
static inline void sys_write_ln(const char* s) {
    sys_write_str(s);
    sys_write_str("\n");
}

// read up to max_len-1 bytes from stdin into buf, null-terminate
static inline long sys_read_line(char* buf, long max_len) {
    long n = sys_call(SYS_READ, 0, (long)buf, max_len - 1);
    if (n <= 0) { if (n == 0) buf[0] = '\0'; return n; }
    // replace trailing newline with '\0'
    long i = 0;
    if (n > 0) {
        if (buf[n-1] == '\n') buf[n-1] = '\0';
        else buf[n] = '\0';
    }
    return n;
}

// write number (signed) to stdout
static inline void sys_write_num(long num) {
    char tmp[32];
    int pos = 31;
    tmp[31] = '\0';
    bool neg = false;
    if (num == 0) { sys_write_fd(1, "0", 1); return; }
    if (num < 0) { neg = true; num = -num; }
    while (num > 0 && pos > 0) {
        tmp[--pos] = '0' + (num % 10);
        num /= 10;
    }
    if (neg) tmp[--pos] = '-';
    sys_write_fd(1, &tmp[pos], 31 - pos);
}

#endif // IO_SYS_H
