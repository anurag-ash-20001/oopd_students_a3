#ifndef SYSCALLS_H
#define SYSCALLS_H

// Linux x86_64 syscall wrapper supporting up to 6 args.
// Uses the calling conventions for syscall: rax=nr, rdi, rsi, rdx, r10, r8, r9

static inline long sys_call6(long nr,
                             long a1, long a2, long a3,
                             long a4, long a5, long a6) {
    long ret;
    register long r10 asm("r10") = a4;
    register long r8  asm("r8")  = a5;
    register long r9  asm("r9")  = a6;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(a1), "S"(a2), "d"(a3),
          "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory"
    );
    return ret;
}

// A convenience 3-arg wrapper (calls the 6-arg one with zeros)
static inline long sys_call(long nr, long a1, long a2, long a3) {
    return sys_call6(nr, a1, a2, a3, 0, 0, 0);
}

// Syscall numbers (linux x86_64)
#define SYS_READ    0
#define SYS_WRITE   1
#define SYS_OPEN    2
#define SYS_CLOSE   3
#define SYS_STAT    4
#define SYS_FSTAT   5
#define SYS_LSEEK   8
#define SYS_MMAP    9
#define SYS_MUNMAP  11
#define SYS_EXIT    60

// mmap flags/prot (simple)
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define MAP_PRIVATE 0x02
#define MAP_ANONYMOUS 0x20
#define MAP_FAILED ((void*) -1)

static inline void* sys_mmap(void* addr, long length, int prot, int flags, int fd, long offset) {
    long ret = sys_call6(SYS_MMAP, (long)addr, length, prot, flags, fd, offset);
    return (void*) ret;
}

static inline long sys_munmap(void* addr, long length) {
    return sys_call6(SYS_MUNMAP, (long)addr, length, 0, 0, 0, 0);
}

#endif // SYSCALLS_H
