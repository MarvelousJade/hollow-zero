#ifndef ATLAS_H
#define ATLAS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <string>
#include <stdio.h>

class Atlas {
    std::vector<SDL_Texture*> m_textureList; 
public:
    Atlas() = default;

    ~Atlas() = default;

    // void load(const char* pathTemplate, int num) {
    //     AssetManager* assetManager = AssetManager::instance();
    //     for (int i = 0; i < num; i++) {
    //         char filePath[255];
    //         std::snprintf(filePath, sizeof(filePath), pathTemplate, i + 1);
    //         SDL_Texture* texture = assetManager->findTexture(filePath);
    //
    //         if( texture == NULL ) {
    //             printf( "Unable to find texture for %s!\n", filePath);
    //         } else {
    //             m_textureList.push_back(texture);
    //         } 
    //
    //     }
    // }

    void clear() {
        m_textureList.clear();
    }

    int getSize() const {
        return (int)m_textureList.size();
    }

    SDL_Texture* getTexture(int idx) const {
        if (idx < 0 || idx >= m_textureList.size()) return nullptr;

        return m_textureList[idx];
    }

    void addTexture(SDL_Texture* texture) {
        m_textureList.push_back(texture);
    }
};

#endif
