#include "AssetManager.h"
#include "Atlas.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <string>
#include <vector>

AssetManager* AssetManager::assetManager = nullptr;

struct AtlasAssetInfo {
    std::string name;
    std::string basePath;
    int frameCount = 0;

};

AssetManager::AssetManager() = default;

AssetManager::~AssetManager() = default;

AssetManager* AssetManager::instance() {
    if (!assetManager) assetManager = new AssetManager();

    return assetManager;
}

Mix_Chunk* AssetManager::findAudio(const std::string& name) {
    const auto& it = audioPool.find(name); 

    if (it == audioPool.end()) return nullptr;

    return it->second;
}

SDL_Texture* AssetManager::findTexture(const std::string& name) {
    const auto& it = texturePool.find(name); 

    if (it == texturePool.end()) return nullptr;

    return it->second;
}

Atlas* AssetManager::findAtlas(const std::string& name) {
    const auto& it = atlasPool.find(name); 

    if (it == atlasPool.end()) return nullptr;

    return it->second;
    
}

void AssetManager::load(SDL_Renderer* renderer) {
    using namespace std::filesystem;
    path assetsPath("assets");

    for (const auto& entry : recursive_directory_iterator(assetsPath)) {
        if (entry.is_regular_file()) {
            const path& filePath = entry.path();
            path relativePath = relative(filePath, assetsPath);
            std::string key = relativePath.replace_extension("").string();

            if (filePath.extension() == ".png") {
                SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.string().c_str());

                if( texture == NULL ) {
                    throw std::runtime_error(
                        std::format("Unable to create texture from {}! SDL Error: {}", filePath.string(), SDL_GetError()) 
                    );
                } else {
                    texturePool[key] = texture;
                }

            } else if (filePath.extension() == ".mp3") {
                Mix_Chunk* audio = Mix_LoadWAV(filePath.string().c_str());

                if( audio == NULL ) {
                    throw std::runtime_error(
                        std::format("Unable to create Mix Chunk from {}! SDL Error: {}", filePath.string(), SDL_GetError()) 
                    );
                } else {
                    audioPool[key] = audio;
                }
            }
        }
    }

    loadEnemyAtlases();
}

std::vector<AtlasAssetInfo> AssetManager::discoverEnemyAtlases() {
    using namespace std::filesystem;
    std::vector<AtlasAssetInfo> atlases;

    path enemyPath("assets/enemy");

    for (const auto& entry : directory_iterator(enemyPath)) {
        if (entry.is_directory()) {
            std::string atlasName = entry.path().filename().string();
            std::string basePath = entry.path().string();

            int frameCount = 0;
            for (const auto& file : directory_iterator(entry)) {
                if (file.path().extension() == ".png") {
                    frameCount++;
                }
            }

            if (frameCount > 0) {
                atlases.push_back({atlasName, basePath, frameCount});
            }
        }
    }

    return atlases;
};

Atlas* AssetManager::createAtlas(const std::vector<std::string>& textureNames) {
    Atlas* atlas = new Atlas();
    
    for (const std::string& name : textureNames) {
        SDL_Texture* texture = findTexture(name);
        if( texture == NULL ) {
            throw std::runtime_error(
                std::format("Unable to find texture for name: {}!", name) 
            );
            } else {
                atlas->addTexture(texture);
            }
    }
    
    return atlas;
}

Atlas* AssetManager::createAtlasByPattern(const std::string& basePath, int count) {
    Atlas* atlas = new Atlas();
    
    for (int i = 1; i <= count; i++) {
        
        std::filesystem::path fullPath(basePath + "/" + std::to_string(i) + ".png");
        std::filesystem::path relativePath = std::filesystem::relative(fullPath, "assets");
        std::string key = relativePath.replace_extension("").string();

        SDL_Texture* texture = findTexture(key);

        if( texture == NULL ) {
            throw std::runtime_error(
                std::format("Unable to find texture for texture name: {}!", key) 
            );
        } else {
            atlas->addTexture(texture);
        }
    }
    
    return atlas;
}

void AssetManager::loadEnemyAtlases() {
    auto discoveredAtlases = discoverEnemyAtlases();

    for (const auto& info : discoveredAtlases) {
        atlasPool[info.name] = createAtlasByPattern(info.basePath, info.frameCount);
    }
}
