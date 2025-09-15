#ifndef __RECOMPDATAWRAPPERS_U32VALUEHASHMAP_H__
#define __RECOMPDATAWRAPPERS_U32VALUEHASHMAP_H__

#include <type_traits>
#include "recompdata.h"

namespace RecompDataWrapper {

// Maps <=32 bit keys to <=32 bit values
template <class K, class V>
class ValueHashmap32 {

private:
    U32ValueHashmapHandle m_handle;

public:
    ValueHashmap32() {
        static_assert((std::is_arithmetic<K>::value || std::is_pointer<K>::value) && !std::is_floating_point<K>::value, "Key type must be an integer type or pointer!");
        static_assert((std::is_arithmetic<V>::value || std::is_pointer<V>::value) && !std::is_floating_point<V>::value, "Value type must be an integer type or pointer!");
        static_assert(sizeof(K) <= sizeof(collection_key_t), "Key type cannot fit into collection_key_t!");
        static_assert(sizeof(V) <= sizeof(unsigned long), "Value type cannot fit into unsigned long!");

        m_handle = recomputil_create_u32_value_hashmap();
    };

    ~ValueHashmap32() { recomputil_destroy_u32_value_hashmap(m_handle); };

    int insert(K key, V val) { return recomputil_u32_value_hashmap_insert(m_handle, (collection_key_t)key, (unsigned long)val); };

    int get(K key, V &out) {
        unsigned long realOut = (unsigned long)out;
        int result = recomputil_u32_value_hashmap_get(m_handle, (collection_key_t)key, &realOut);
        out = (V)realOut;
        return result;
    };

    int contains(K key) { return recomputil_u32_value_hashmap_contains(m_handle, (collection_key_t)key); };

    int erase(K key) { return recomputil_u32_value_hashmap_erase(m_handle, (collection_key_t)key); };

    unsigned long size() { return recomputil_u32_value_hashmap_size(m_handle); };

    void clear() {
        recomputil_destroy_u32_value_hashmap(m_handle);
        m_handle = recomputil_create_u32_value_hashmap();
    };

    // Raw handle for the underlying recomp data structure. Must NOT have the destroy function called on it.
    //
    // If the clear method is called, all previously returned handles are considered invalid.
    U32ValueHashmapHandle handle() {
        return m_handle;
    }
};

using U32ValueHashmap = ValueHashmap32<collection_key_t, unsigned long>;

} // namespace RecompDataWrapper

#endif
