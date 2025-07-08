#ifndef UTILS_H
#define UTILS_H

#include "AssetManager.h"

#include <random>
#include <iostream>

inline int playAudio(const std::string& name, bool shouldLoop) {
    Mix_Chunk* chunk = AssetManager::instance()->findAudio(name);
    std::cout << name << " Mix Chunk: " << (chunk ? "OK" : "NULL") << std::endl;

    if (chunk) {
        int loops = shouldLoop ? -1 : 0; // -1 for infinite loop, 0 for play once
        return Mix_PlayChannel(-1, chunk, loops);
    }
    return -1; // Failed to play
}

inline int randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

#endif
