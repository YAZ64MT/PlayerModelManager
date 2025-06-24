#include "recomputils.h"
#include "dynmemarray.h"

#define DEFAULT_CAPACITY 16
#define NEXT_CAPACITY(current) ((current == 0) ? DEFAULT_CAPACITY : ((current + 1) * 3 / 2))

void resizeDynMemArr(DynamicMemoryArray *dArr, size_t newCapacity) {
    if (newCapacity > 0) {
        size_t newByteSize = newCapacity * dArr->elementSize;
        u32 *newData = recomp_alloc(newByteSize);
        Lib_MemSet(newData, 0, newByteSize);

        size_t min = dArr->count;
        if (min > newCapacity) {
            min = newCapacity;
        }

        size_t newCount = dArr->count;
        if (newCapacity < dArr->count) {
            newCount = newCapacity;
        }

        if (dArr->data) {
            Lib_MemCpy(newData, dArr->data, min * dArr->elementSize);
            recomp_free(dArr->data);
        }

        dArr->count = newCount;
        dArr->capacity = newCapacity;
        dArr->data = newData;
    }
}

void resetStruct(DynamicMemoryArray *dArr) {
    dArr->capacity = 0;
    dArr->count = 0;
    dArr->elementSize = 0;
    dArr->data = NULL;
}

void DynMemArr_init(DynamicMemoryArray *dArr, size_t elementSize, size_t initialCapacity) {
    resetStruct(dArr);

    dArr->elementSize = elementSize;

    if (initialCapacity) {
        resizeDynMemArr(dArr, initialCapacity);
    }
}

void DynMemArr_clear(DynamicMemoryArray *dArr) {
    dArr->count = 0;
}

void DynMemArr_destroyMembers(DynamicMemoryArray *dArr) {
    recomp_free(dArr->data);
    resetStruct(dArr);
}

void *DynMemArr_createElement(DynamicMemoryArray *dArr) {
    if (dArr->elementSize < 1) {
        recomp_printf("DynMemArr_createElement: Element size is 0!\nIgnoring...\n");
        return NULL;
    }

    size_t newCount = dArr->count + 1;

    if (newCount > dArr->capacity) {
        resizeDynMemArr(dArr, NEXT_CAPACITY(dArr->capacity));
    }

    u8 *data = dArr->data;

    void *element = DynMemArr_get(dArr, dArr->count);

    dArr->count = newCount;

    return element;
}

void *DynMemArr_get(DynamicMemoryArray *dArr, size_t index) {
    if (dArr->elementSize < 1) {
        recomp_printf("DynMemArr_get: Element size is 0!\nIgnoring...\n");
        return NULL;
    }

    u8 *data = dArr->data;

    return &data[dArr->elementSize * index];
}

void DynMemArr_remove(DynamicMemoryArray *dArr, size_t index) {
    if (dArr->elementSize < 1) {
        recomp_printf("DynMemArr_get: Element size is 0!\nIgnoring...\n");
        return;
    }

    if (dArr->count == 0) {
        return;
    }

    u8 *data = dArr->data;

    for (size_t i = index + 1; i < dArr->count - 1; ++i) {
        Lib_MemCpy(DynMemArr_get(dArr, i), DynMemArr_get(dArr, i + 1), dArr->elementSize);
    }

    dArr->count--;
}
