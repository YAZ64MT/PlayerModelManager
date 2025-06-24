#ifndef __DYNVALUEARRAY_H__
#define __DYNVALUEARRAY_H__

#include "global.h"

typedef struct {
    void *data;
    size_t capacity;
    size_t count;
    size_t elementSize;
} DynamicMemoryArray;

void DynMemArr_init(DynamicMemoryArray *dArr, size_t elementSize, size_t initialCapacity);

void DynMemArr_clear(DynamicMemoryArray *dArr);

void DynMemArr_destroyMembers(DynamicMemoryArray *dArr);

void *DynMemArr_createElement(DynamicMemoryArray *dArr);

void *DynMemArr_get(DynamicMemoryArray *dArr, size_t index);

void DynMemArr_remove(DynamicMemoryArray *dArr, size_t index);

#endif