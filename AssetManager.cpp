#include "AssetManager.h"
#include "Atlas.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

AssetManager* AssetManager::s_assetManager = nullptr;

struct AtlasAssetInfo {
    std::string name;
    std::string basePath;
    int frameCount = 0;

};

AssetManager::AssetManager() = default;

AssetManager::~AssetManager() {
    for (auto& [name, atlas] : atlasPool) {
        delete atlas;
    }
    atlasPool.clear();

    for (auto& [name, texture] : texturePool) {
        SDL_DestroyTexture(texture);
    }
    texturePool.clear();

    for (auto& [name, chunk] : audioPool) {
        Mix_FreeChunk(chunk);
    }
    audioPool.clear();
};

AssetManager* AssetManager::instance() {
    if (!s_assetManager) s_assetManager = new AssetManager();

    return s_assetManager;
}

Mix_Chunk* AssetManager::findAudio(const std::string& name) {
    const auto& it = audioPool.find(name); 

    if (it == audioPool.end()) {
        std::cerr << "Warning: Can't find audio: " << name << std::endl;
        return nullptr;
    }

    return it->second;
}

SDL_Texture* AssetManager::findTexture(const std::string& name) {
    const auto& it = texturePool.find(name); 

    if (it == texturePool.end()) {
        std::cerr << "Warning: Can't find texture: " << name << std::endl;
        return nullptr;
    }

    return it->second;
}

Atlas* AssetManager::findAtlas(const std::string& name) {
    const auto& it = atlasPool.find(name); 

    if (it == atlasPool.end()) {
        std::cerr << "Warning: Can't find atlas: " << name << std::endl;
        return nullptr;
    }

    return it->second;
    
}

void AssetManager::load(SDL_Renderer* renderer) {
    using namespace std::filesystem;

    if (!renderer) {
        throw std::runtime_error("Renderer is null!");
    }

    // std::cout << "Loading assets..." << std::endl;
    path assetsPath("assets");
    if (!exists(assetsPath)) {
        throw std::runtime_error("Assets directory not found!");
    }

    // std::cout << "Loading assets from: " << absolute(assetsPath) << std::endl;
    loadTexturesAndAudio(renderer, assetsPath);
    loadAtlases();
    
    // std::cout << "Asset loading complete!" << std::endl;
    // std::cout << "  Textures loaded: " << texturePool.size() << std::endl;
    // std::cout << "  Audio loaded: " << audioPool.size() << std::endl;
    // std::cout << "  Atlases created: " << atlasPool.size() << std::endl;
}

void AssetManager::loadTexturesAndAudio(SDL_Renderer* renderer, const std::filesystem::path& assetsPath) {
    using namespace std::filesystem;
    
    for (const auto& entry : recursive_directory_iterator(assetsPath)) {
        if (!entry.is_regular_file()) continue;
        
        const path& filePath = entry.path();
        path relativePath = relative(filePath, assetsPath);
        
        // Normalize path separators to forward slashes for consistency
        std::string key = relativePath.generic_string();
        
        // Remove extension
        size_t lastDot = key.find_last_of('.');
        if (lastDot != std::string::npos) {
            key = key.substr(0, lastDot);
        }

        if (filePath.extension() == ".png") {
            loadTexture(renderer, filePath, key);
        } else if (filePath.extension() == ".mp3") {
            loadAudio(filePath, key);
        }
    }
} 

void AssetManager::loadTexture(SDL_Renderer* renderer, const std::filesystem::path& filePath, const std::string& key) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.string().c_str());
    
    if (!texture) {
        std::cerr << "Warning: Failed to load texture " << filePath << ": " << IMG_GetError() << std::endl;
        return;
    }
    
    texturePool[key] = texture;
}

void AssetManager::loadAudio(const std::filesystem::path& filePath, const std::string& key) {
    Mix_Chunk* audio = Mix_LoadWAV(filePath.string().c_str());
    
    if (!audio) {
        std::cerr << "Warning: Failed to load audio " << filePath << ": " << Mix_GetError() << std::endl;
        return;
    }
    
    audioPool[key] = audio;
}

void AssetManager::loadAtlases() {
    // Load enemy atlases (directories with numbered PNG files)
    loadEnemyAtlases();
}

void AssetManager::loadEnemyAtlases() {
    auto discoveredAtlases = discoverAtlases("enemy");
    
    for (const auto& info : discoveredAtlases) {
        Atlas* atlas = createAtlasFromDirectory(info);
        if (atlas && atlas->getSize() > 0) {
            atlasPool[info.name] = atlas;
        } else {
            delete atlas;
            std::cerr << "Warning: Failed to create atlas for " << info.name << std::endl;
        }
    }
}

std::vector<AtlasAssetInfo> AssetManager::discoverAtlases(const std::string& subdir) {
    using namespace std::filesystem;
    std::vector<AtlasAssetInfo> atlases;
    
    path searchPath = path("assets") / subdir;
    
    if (!exists(searchPath)) {
        std::cerr << "Warning: Directory " << searchPath << " does not exist" << std::endl;
        return atlases;
    }

    for (const auto& entry : directory_iterator(searchPath)) {
        if (!entry.is_directory()) continue;
        
        AtlasAssetInfo info;
        info.basePath = entry.path().string();
        
        // Create atlas name as relative path from assets directory
        info.name = (path(subdir) / entry.path().filename()).generic_string();
        
        // Count PNG files and verify they're numbered sequentially
        std::vector<int> frameNumbers;
        for (const auto& file : directory_iterator(entry)) {
            if (file.path().extension() == ".png") {
                std::string filename = file.path().stem().string();
                try {
                    int frameNum = std::stoi(filename);
                    frameNumbers.push_back(frameNum);
                } catch (...) {
                    std::cerr << "Warning: Non-numeric filename in atlas directory: " << file.path() << std::endl;
                }
            }
        }
        
        if (!frameNumbers.empty()) {
            std::sort(frameNumbers.begin(), frameNumbers.end());
            info.frameCount = frameNumbers.size();
            
            // Verify frames are sequential starting from 1
            bool isSequential = true;
            for (size_t i = 0; i < frameNumbers.size(); ++i) {
                if (frameNumbers[i] != static_cast<int>(i + 1)) {
                    isSequential = false;
                    break;
                }
            }
            
            if (!isSequential) {
                std::cerr << "Warning: Non-sequential frame numbers in " << info.name << std::endl;
            }
            
            atlases.push_back(info);
        }
    }
    
    return atlases;
}

Atlas* AssetManager::createAtlasFromDirectory(const AtlasAssetInfo& info) {
    Atlas* atlas = new Atlas();
    
    for (int i = 1; i <= info.frameCount; ++i) {
        // Build the texture key
        std::string key = info.name + "/" + std::to_string(i);
        
        SDL_Texture* texture = findTexture(key);
        if (texture) {
            atlas->addTexture(texture);
        } else {
            std::cerr << "Warning: Missing texture for atlas frame: " << key << std::endl;
        }
    }
    
    return atlas;
}

Atlas* AssetManager::createAtlas(const std::vector<std::string>& textureNames) {
    Atlas* atlas = new Atlas();
    
    for (const std::string& name : textureNames) {
        SDL_Texture* texture = findTexture(name);
        if (texture) {
            atlas->addTexture(texture);
        } else {
            std::cerr << "Warning: Texture not found for atlas: " << name << std::endl;
        }
    }
    
    return atlas;
}
