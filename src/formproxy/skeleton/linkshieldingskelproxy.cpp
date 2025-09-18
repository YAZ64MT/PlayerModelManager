#include "formproxy/skeleton/linkshieldskelproxy.hpp"

yazmt::pmm::LinkShieldSkelProxy::LinkShieldSkelProxy(std::array<StandardLimb, NUM_LIMBS> src) {
    size_t numDLs = 0;

    for (size_t i = 0; i < limbs.size(); ++i) {
        limbs[i] = src[i];
        if (src[i].dList) {
            numDLs++;
        }

        limbPtrs[i] = static_cast<void *>(&limbs[i]);
    }

    skel.dListCount = numDLs;
    skel.sh.limbCount = limbs.size();
    skel.sh.segment = limbPtrs.data();
}
