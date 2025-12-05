// argstack.h
#pragma once
#include <stdlib.h>

typedef struct {
    void **items;
    int size;
    int capacity;
} ArgStack;

static inline ArgStack* argstack_create(int capacity) {
    ArgStack* s = (ArgStack*)malloc(sizeof(ArgStack));
    s->items = (void**)malloc(sizeof(void*) * capacity);
    s->size = 0;
    s->capacity = capacity;
    return s;
}

static inline void argstack_free(ArgStack* s) {
    free(s->items);
    free(s);
}

static inline void argstack_push_ptr(ArgStack* s, void* p) {
    if (s->size < s->capacity) s->items[s->size++] = p;
}

static inline void argstack_push_i32(ArgStack* s, int v) {
    int *pv = (int*)malloc(sizeof(int));
    *pv = v;
    argstack_push_ptr(s, pv);
}

static inline void argstack_push_f32(ArgStack* s, float v) {
    float *pv = (float*)malloc(sizeof(float));
    *pv = v;
    argstack_push_ptr(s, pv);
}

static inline void argstack_push_f64(ArgStack* s, double v) {
    double *pv = (double*)malloc(sizeof(double));
    *pv = v;
    argstack_push_ptr(s, pv);
}

static inline void argstack_push_cstring(ArgStack* s, const char* str) {
    argstack_push_ptr(s, (void*)str); // just store pointer, no copy
}

// expose "view" for call_func
static inline void** argstack_data(ArgStack* s) { return s->items; }
static inline int argstack_size(ArgStack* s) { return s->size; }
