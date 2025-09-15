#ifndef __RECOMPDATAWRAPPERS_U32MEMORYHASHMAP_H__
#define __RECOMPDATAWRAPPERS_U32MEMORYHASHMAP_H__

#include <type_traits>
#include "recompdata.h"

namespace RecompDataWrapper {

// Maps <=32 bit keys to types of arbitrary size
template <class K, class V>
class MemoryHashmap32 {

private:
    U32MemoryHashmapHandle m_handle;

public:
    MemoryHashmap32() {
        static_assert((std::is_arithmetic<K>::value || std::is_pointer<K>::value) && !std::is_floating_point<K>::value, "Key type must be an integer type or pointer!");
        static_assert(sizeof(K) <= sizeof(collection_key_t), "Key type cannot fit into collection_key_t!");
        static_assert(sizeof(V) > 0, "Value type must have a size greater than 0!");

        m_handle = recomputil_create_u32_memory_hashmap(sizeof(V));
    };

    ~MemoryHashmap32() { recomputil_destroy_u32_memory_hashmap(m_handle); };

    int create(K key) { return recomputil_u32_memory_hashmap_create(m_handle, (collection_key_t)key); };

    V *get(K key) { return static_cast<V *>(recomputil_u32_memory_hashmap_get(m_handle, (collection_key_t)key)); };

    int contains(K key) { return recomputil_u32_memory_hashmap_contains(m_handle, (collection_key_t)key); };

    int erase(K key) { return recomputil_u32_memory_hashmap_erase(m_handle, (collection_key_t)key); };

    unsigned long size() { return recomputil_u32_memory_hashmap_size(m_handle); };

    void clear() {
        recomputil_destroy_u32_memory_hashmap(m_handle);
        m_handle = recomputil_create_u32_memory_hashmap(sizeof(V));
    }

    // Raw handle for the underlying recomp data structure. Must NOT have the destroy function called on it.
    //
    // If the clear method is called, all previously returned handles are considered invalid.
    U32MemoryHashmapHandle handle() {
        return m_handle;
    }
};

template <class V>
using U32MemoryHashmap = MemoryHashmap32<collection_key_t, V>;

} // namespace RecompDataWrapper

#endif
