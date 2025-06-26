#ifndef __DYNU32ARRAY_H__
#define __DYNU32ARRAY_H__

#include "global.h"

typedef struct {
    u32 *data;
    size_t capacity;
    size_t count;
} DynamicU32Array;

void DynU32Arr_init(DynamicU32Array *dArr);

void DynU32Arr_clear(DynamicU32Array *dArr);

void DynU32Arr_destroyMembers(DynamicU32Array *dArr);

void DynU32Arr_push(DynamicU32Array *dArr, u32 value);

#endif
