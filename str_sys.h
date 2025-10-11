#ifndef STR_SYS_H
#define STR_SYS_H

#include "io_sys.h"
#include "stddef_sys.h"

// strlen
static inline long sys_strlen(const char* s) {
    long i = 0;
    while (s[i]) ++i;
    return i;
}

// strcpy (null-terminated)
static inline void sys_strcpy(char* dst, const char* src) {
    while ((*dst++ = *src++));
}

// strcmp
static inline int sys_strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) { ++a; ++b; }
    return (unsigned char)*a - (unsigned char)*b;
}

// strcat
static inline void sys_strcat(char* dst, const char* src) {
    while (*dst) ++dst;
    while ((*dst++ = *src++));
}

// duplicate string (uses simple brk/mmap not implemented; use static buffer by caller)

// parse double minimal (handles integers and decimals, no scientific)
static inline double sys_atof(const char* s) {
    double sign = 1.0;
    double val = 0.0;
    const char* p = s;
    if (*p == '+') ++p;
    else if (*p == '-') { sign = -1.0; ++p; }
    while (*p >= '0' && *p <= '9') {
        val = val * 10.0 + (*p - '0');
        ++p;
    }
    if (*p == '.' ) {
        ++p;
        double frac = 0.0;
        double base = 1.0;
        while (*p >= '0' && *p <= '9') {
            frac = frac * 10.0 + (*p - '0');
            base *= 10.0;
            ++p;
        }
        val += frac / base;
    }
    return sign * val;
}

// integer to ascii into dest; returns pointer after written string (null-terminated)
static inline char* sys_itoa(long val, char* dest) {
    char tmp[32];
    int pos = 0;
    bool neg = false;
    if (val == 0) { dest[0] = '0'; dest[1] = '\0'; return dest + 1; }
    if (val < 0) { neg = true; val = -val; }
    while (val > 0) {
        tmp[pos++] = '0' + (val % 10);
        val /= 10;
    }
    int idx = 0;
    if (neg) dest[idx++] = '-';
    for (int i = pos - 1; i >= 0; --i) dest[idx++] = tmp[i];
    dest[idx] = '\0';
    return dest + idx + 1;
}

// double to ascii minimal (integer part . fractional 2 digits)
static inline void sys_dtoa2(double v, char* buf) {
    long ip = (long)v;
    if (v < 0) { *buf++ = '-'; v = -v; ip = (long)v; }
    // integer
    char* p = buf;
    p = sys_itoa(ip, p);
    // fractional two digits
    *p++ = '.';
    int frac2 = (int)((v - (double)ip) * 100 + 0.5);
    if (frac2 < 0) frac2 = 0;
    if (frac2 < 10) { *p++ = '0'; sys_itoa(frac2, p); }
    else sys_itoa(frac2, p);
}

#endif // STR_SYS_H
