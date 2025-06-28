#ifndef __STRINGU32DICTIONARY_H__
#define __STRINGU32DICTIONARY_H__

#include "ultra64.h"
#include "recompdata.h"

typedef U32MemoryHashmapHandle StringU32DictionaryHandle;

// Creates a U32ValueDictionary and returns a handle
StringU32DictionaryHandle StringU32Dictionary_create();

// Returns true if key was created, false otherwise.
bool StringU32Dictionary_set(StringU32DictionaryHandle dict, const char *key, u32 value);

// Destroys a U32ValueDictionary
void StringU32Dictionary_destroy(StringU32DictionaryHandle dict);

// Returns true and writes to out if key exists, false and leaves out unmodified otherwise
bool StringU32Dictionary_get(StringU32DictionaryHandle dict, const char *key, u32 *out);

// Returns true if key existed and was removed, false otherwise
bool StringU32Dictionary_unset(StringU32DictionaryHandle dict, const char *key);

// Returns true if key exists in dictionary, false otherwise
bool StringU32Dictionary_has(StringU32DictionaryHandle dict, const char *key);

#endif
