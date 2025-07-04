#ifndef UTILS_H
#define UTILS_H

#include "AssetManager.h"

#include <random>

inline void playAudio(const std::string& name, bool shouldLoop) {
    Mix_Chunk* chunk = AssetManager::instance()->findAudio(name);
    Mix_PlayChannel(-1, chunk, shouldLoop);
}

inline int randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

#endif
