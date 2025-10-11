#ifndef STDDEF_SYS_H
#define STDDEF_SYS_H

// Define size_t as unsigned long (standard for 64-bit Linux)
typedef unsigned long size_t;

// Define NULL
#ifndef NULL
#define NULL ((void*)0)
#endif

// Define offsetof macro
#define offsetof(type, member) ((size_t) &(((type*)0)->member))

#endif // STDDEF_SYS_H
