#ifndef __MODELINFO_HPP__
#define __MODELINFO_HPP__

#include <array>

// Define `this` before including decomp headers to work around it being a keyword in C++.
#define this this_
extern "C" {
#include "z64player.h"
#include "modelmatrixids.h"
}
// Undo the `this` define.
#undef this

namespace yazmt {
namespace pmm {

class ModelInfo {
public:
    const static u64 FLAG_IS_USE_ADULT_FIX = 0x1;

    u64 flags;
    FlexSkeletonHeader *skeleton;
    FlexSkeletonHeader *shieldingSkeleton;
    std::array<Gfx *, LINK_DL_MAX> models;
    std::array<Mtx *, LINK_EQUIP_MATRIX_MAX> matrixes;
    std::array<TexturePtr, PLAYER_EYES_MAX> eyesTextures;
    std::array<TexturePtr, PLAYER_MOUTH_MAX> mouthTextures;

    ModelInfo() { clear(false); }

    void clear(bool isFaceTexturesPreserved = false);

    void clearFlags();

    void clearEyes();

    void clearMouths();

    void clearSkeletons();

    void clearModels();

    void clearMatrixes();
    
    void setEyes(TexturePtr textures[], size_t n);

    void setMouths(TexturePtr textures[], size_t n);

    bool isAdultFixesEnabled() { return flags & FLAG_IS_USE_ADULT_FIX; };
};

} // namespace pmm
} // namespace yazmt

#endif
