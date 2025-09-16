// math.h must be included before any decomp headers to prevent issues that stem from defining `this`.
#include "math.h"

// Define `this` before including decomp headers to work around it being a keyword in C++.
#define this this_
extern "C" {
#include "global.h"
}
// Undo the `this` define.
#undef this

#include "formproxy/modelinfo.hpp"

void yazmt::pmm::ModelInfo::clear(bool isFaceTexturesPreserved) {
    clearFlags();
    clearSkeletons();
    clearModels();
    clearMatrixes();
    if (!isFaceTexturesPreserved) {
        clearEyes();
        clearMouths();
    }
}

void yazmt::pmm::ModelInfo::clearFlags() {
    flags = 0;
}

void yazmt::pmm::ModelInfo::clearEyes() {
    for (size_t i = 0; i < eyesTextures.size(); ++i) {
        eyesTextures[i] = nullptr;
    }
}

void yazmt::pmm::ModelInfo::clearMouths() {
    for (size_t i = 0; i < mouthTextures.size(); ++i) {
        mouthTextures[i] = nullptr;
    }
}

void yazmt::pmm::ModelInfo::clearSkeletons() {
    skeleton = nullptr;
    shieldingSkeleton = nullptr;
}

void yazmt::pmm::ModelInfo::clearModels() {
    for (size_t i = 0; i < models.size(); ++i) {
        models[i] = nullptr;
    }
}

void yazmt::pmm::ModelInfo::clearMatrixes() {
    for (size_t i = 0; i < matrixes.size(); ++i) {
        matrixes[i] = nullptr;
    }
}

void yazmt::pmm::ModelInfo::setEyes(TexturePtr textures[], size_t n) {
    clearEyes();

    size_t min = n > eyesTextures.size() ? eyesTextures.size() : n;

    for (size_t i = 0; i < min; ++i) {
        eyesTextures[i] = textures[i];
    }
}

void yazmt::pmm::ModelInfo::setMouths(TexturePtr textures[], size_t n) {
    clearMouths();

    size_t min = n > mouthTextures.size() ? mouthTextures.size() : n;

    for (size_t i = 0; i < min; ++i) {
        mouthTextures[i] = textures[i];
    }
}
