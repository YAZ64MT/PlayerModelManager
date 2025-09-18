#ifndef __LINKSHIELDSKELPROXY_HPP__
#define __LINKSHIELDSKELPROXY_HPP__

// math.h must be included before any decomp headers to prevent issues that stem from defining `this`.
#include "math.h"

// Define `this` before including decomp headers to work around it being a keyword in C++.
#define this this_
extern "C" {
#include "global.h"
}
// Undo the `this` define.
#undef this

#include <array>

namespace yazmt {
namespace pmm {

class LinkShieldSkelProxy {
public:
    static const size_t NUM_LIMBS = 4;

    FlexSkeletonHeader skel;
    std::array<StandardLimb, NUM_LIMBS> limbs;
    std::array<void *, NUM_LIMBS> limbPtrs;

    LinkShieldSkelProxy(std::array<StandardLimb, NUM_LIMBS> src);
};

} // namespace pmm
} // namespace yazmt

#endif
