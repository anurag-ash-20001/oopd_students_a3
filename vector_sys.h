#ifndef VECTOR_SYS_H
#define VECTOR_SYS_H

#include "syscalls.h"

// Minimal SysVector that allocates pointer-array with sys_mmap.
// This version **checks** MAP_FAILED and fails gracefully (returns without crash).

typedef struct {
    void** data;
    long size;
    long capacity;
} SysVector;

// allocate pointer array via mmap (returns nullptr on failure)
static inline void* ptrarray_alloc(long n) {
    long bytes = n * (long)sizeof(void*);
    void* p = sys_mmap(0, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) return 0;
    return p;
}

// free pointer array via munmap
static inline void ptrarray_free(void* p, long n) {
    if (!p) return;
    long bytes = n * (long)sizeof(void*);
    sys_munmap(p, bytes);
}

static inline void vector_init(SysVector* v, long init_cap) {
    if (init_cap < 4) init_cap = 4;
    v->size = 0;
    v->capacity = init_cap;
    v->data = (void**) ptrarray_alloc(init_cap);
    if (!v->data) {
        // allocation failed; set to safe null state
        v->capacity = 0;
        v->size = 0;
    }
}

static inline void vector_push(SysVector* v, void* elem) {
    if (!v->data) {
        vector_init(v, 4);
        if (!v->data) return; // still failed
    }
    if (v->size >= v->capacity) {
        long newcap = v->capacity ? v->capacity * 2 : 4;
        void** newdata = (void**) ptrarray_alloc(newcap); // FIX: cast to void**
        if (!newdata) return;

        // manual copy loop
        for (long i = 0; i < v->size; ++i)
            newdata[i] = v->data[i];

        ptrarray_free(v->data, v->capacity);
        v->data = newdata;
        v->capacity = newcap;
    }

    v->data[v->size++] = elem;
}


static inline void vector_erase_at(SysVector* v, long idx) {
    if (!v->data) return;
    if (idx < 0 || idx >= v->size) return;
    for (long i = idx; i + 1 < v->size; ++i) v->data[i] = v->data[i+1];
    --v->size;
}

static inline void vector_free(SysVector* v) {
    if (v->data) {
        ptrarray_free(v->data, v->capacity);
        v->data = 0;
    }
    v->size = 0;
    v->capacity = 0;
}

#endif // VECTOR_SYS_H
