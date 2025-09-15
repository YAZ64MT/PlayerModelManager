#ifndef __RECOMPDATAWRAPPERS_MEMORYSLOTMAP_H__
#define __RECOMPDATAWRAPPERS_MEMORYSLOTMAP_H__

#include <type_traits>
#include "recompdata.h"

namespace RecompDataWrapper {

// Maps <=32 bit keys to types of arbitrary size
template <class K, class V>
class MemorySlotmap {

private:
    MemorySlotmapHandle m_handle;

public:
    MemorySlotmap() {
        static_assert((std::is_arithmetic<K>::value || std::is_pointer<K>::value) && !std::is_floating_point<K>::value, "Key type must be an integer type or pointer!");
        static_assert(sizeof(K) <= sizeof(collection_key_t), "Key type cannot fit into collection_key_t!");
        static_assert(sizeof(V) > 0, "Value type must have a size greater than 0!");

        m_handle = recomputil_create_memory_slotmap(sizeof(V));
    };

    ~MemorySlotmap() { recomputil_destroy_memory_slotmap(m_handle); };

    K create() { return (K)recomputil_memory_slotmap_create(m_handle); };

    V *get(K key) { return static_cast<V *>(recomputil_memory_slotmap_get(m_handle, (collection_key_t)key)); };

    int contains(K key) { return recomputil_memory_slotmap_contains(m_handle, (collection_key_t)key); };

    int erase(K key) { return recomputil_memory_slotmap_erase(m_handle, (collection_key_t)key); };

    unsigned long size() { return recomputil_memory_slotmap_size(m_handle); };

    void clear() {
        recomputil_destroy_memory_slotmap(m_handle);
        m_handle = recomputil_create_memory_slotmap(sizeof(V));
    }

    // Raw handle for the underlying recomp data structure. Must NOT have the destroy function called on it.
    //
    // If the clear method is called, all previously returned handles are considered invalid.
    MemorySlotmapHandle handle() {
        return m_handle;
    }
};

} // namespace RecompDataWrapper

#endif
