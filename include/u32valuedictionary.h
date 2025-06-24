#ifndef __U32VALUEDICTIONARY_H__
#define __U32VALUEDICTIONARY_H__

#include "ultra64.h"
#include "recompdata.h"

typedef U32MemoryHashmapHandle U32ValueDictionaryHandle;

U32ValueDictionaryHandle U32ValueDictionary_create();

// Returns true if key was created, false otherwise.
bool U32ValueDictionary_set(U32ValueDictionaryHandle dict, const char *key, u32 value);

// Returns true and writes to out if key exists, false and leaves out unmodified otherwise
bool U32ValueDictionary_get(U32ValueDictionaryHandle dict, const char *key, u32 *out);

// Returns true if key existed and was removed, false otherwise
bool U32ValueDictionary_unset(U32ValueDictionaryHandle dict, const char *key);

#endif
