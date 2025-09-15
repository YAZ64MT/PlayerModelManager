#ifndef __RECOMPDATAWRAPPERS_U32HASHSET_H__
#define __RECOMPDATAWRAPPERS_U32HASHSET_H__

#include <type_traits>
#include "recompdata.h"

namespace RecompDataWrapper {

// Set of 32-bit values
template <class T>
class Hashset32 {

private:
    U32HashsetHandle m_handle;

public:
    Hashset32() {
        static_assert((std::is_arithmetic<T>::value || std::is_pointer<T>::value) && !std::is_floating_point<T>::value, "Set type must be an integer type or pointer!");
        static_assert(sizeof(T) <= sizeof(collection_key_t), "Key type cannot fit into collection_key_t!");

        m_handle = recomputil_create_u32_hashset();
    };

    ~Hashset32() { recomputil_destroy_u32_hashset(m_handle); };

    int insert(T val) { return recomputil_u32_hashset_insert(m_handle, (collection_key_t)val); };

    int contains(T val) { return recomputil_u32_hashset_contains(m_handle, (collection_key_t)val); };

    int erase(T val) { return recomputil_u32_hashset_erase(m_handle, (collection_key_t)val); };

    unsigned long size() { return recomputil_u32_hashset_size(m_handle); };

    void clear() {
        recomputil_destroy_u32_hashset(m_handle);
        m_handle = recomputil_create_u32_hashset();
    };

    // Raw handle for the underlying recomp data structure. Must NOT have the destroy function called on it.
    //
    // If the clear method is called, all previously returned handles are considered invalid.
    U32HashsetHandle handle() {
        return m_handle;
    }
};

using U32Hashset = Hashset32<collection_key_t>;

} // namespace RecompDataWrapper

#endif
