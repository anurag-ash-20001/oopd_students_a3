#ifndef EXCEPT_SYS_H
#define EXCEPT_SYS_H

#include "io_sys.h"
#include "syscalls.h"

static inline void sys_error_exit(const char* msg) {
    sys_write_str("Fatal error: ");
    sys_write_ln(msg);
    sys_call(SYS_EXIT, 1, 0, 0);
}

#endif // EXCEPT_SYS_H
