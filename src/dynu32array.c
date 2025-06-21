#include "recomputils.h"
#include "dynu32array.h"

#define DEFAULT_CAPACITY 16
#define NEXT_CAPACITY(current) ((current == 0) ? DEFAULT_CAPACITY : (current * 3 / 2))

void resize(DynamicU32Array *dArr, size_t newSize) {
    if (newSize > 0) {
        u32 *newData = recomp_alloc(sizeof(*newData) * newSize);

        size_t min = dArr->count;
        if (min > newSize) {
            min = newSize;
        }

        size_t newCount = 0;
        for (size_t i = 0; i < min; ++i) {
            newData[i] = dArr->data[i];
            newCount++;
        }

        recomp_free(dArr->data);

        dArr->count = newCount;
        dArr->capacity = newSize;
        dArr->data = newData;
    }
}

void DynU32Arr_clear(DynamicU32Array *dArr) {
    dArr->count = 0;
}

void DynU32Arr_destroyMembers(DynamicU32Array *dArr) {
    recomp_free(dArr->data);
    dArr->capacity = 0;
    dArr->count = 0;
    dArr->data = NULL;
}

void DynU32Arr_push(DynamicU32Array *dArr, u32 value) {
    size_t newCount = dArr->count + 1;

    if (newCount > dArr->capacity) {
        resize(dArr, NEXT_CAPACITY(dArr->capacity));
    }

    dArr->data[dArr->count] = value;

    dArr->count = newCount;
}
