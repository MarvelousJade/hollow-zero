#include "AssetManager.h"
#include "Atlas.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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

    path assetsPath("assets");
#ifndef __EMSCRIPTEN__
    if (!exists(assetsPath)) {
        throw std::runtime_error("Assets directory not found!");
    }
#endif
    loadTexturesAndAudio(renderer, assetsPath);
    loadAtlases();
    
    // std::cout << "Asset loading complete!" << std::endl;
    // std::cout << "  Textures loaded: " << texturePool.size() << std::endl;
    // std::cout << "  Audio loaded: " << audioPool.size() << std::endl;
    // std::cout << "  Atlases created: " << atlasPool.size() << std::endl;
}

#ifdef __EMSCRIPTEN__
void AssetManager::loadTexturesAndAudio(SDL_Renderer* renderer, const std::filesystem::path&) {
    static const std::vector<std::pair<std::string, std::string>> textures = {
        {"assets/background.png", "background"},
        {"assets/ui_heart.png", "ui_heart"},
        {"assets/player/attack.png", "player/attack"},
        {"assets/player/dead.png", "player/dead"},
        {"assets/player/fall.png", "player/fall"},
        {"assets/player/idle.png", "player/idle"},
        {"assets/player/jump.png", "player/jump"},
        {"assets/player/roll.png", "player/roll"},
        {"assets/player/run.png", "player/run"},
        {"assets/player/vfx_attack_down.png", "player/vfx_attack_down"},
        {"assets/player/vfx_attack_left.png", "player/vfx_attack_left"},
        {"assets/player/vfx_attack_right.png", "player/vfx_attack_right"},
        {"assets/player/vfx_attack_up.png", "player/vfx_attack_up"},
        {"assets/player/vfx_jump.png", "player/vfx_jump"},
        {"assets/player/vfx_land.png", "player/vfx_land"},
    };

    // Enemy sprite frames: {directory, frame_count}
    static const std::vector<std::pair<std::string, int>> enemyAtlases = {
        {"aim", 9}, {"barb_break", 3}, {"barb_loose", 5},
        {"dash_in_air", 2}, {"dash_on_floor", 2}, {"fall", 4},
        {"idle", 6}, {"jump", 8}, {"run", 8}, {"silk", 9},
        {"squat", 10}, {"sword", 3}, {"throw_barb", 8},
        {"throw_silk", 17}, {"throw_sword", 16},
        {"vfx_dash_in_air", 5}, {"vfx_dash_on_floor", 6},
    };

    static const std::vector<std::pair<std::string, std::string>> audios = {
        {"assets/audio/barb_break.mp3", "audio/barb_break"},
        {"assets/audio/bgm.mp3", "audio/bgm"},
        {"assets/audio/bullet_time.mp3", "audio/bullet_time"},
        {"assets/audio/enemy_dash.mp3", "audio/enemy_dash"},
        {"assets/audio/enemy_hurt_1.mp3", "audio/enemy_hurt_1"},
        {"assets/audio/enemy_hurt_2.mp3", "audio/enemy_hurt_2"},
        {"assets/audio/enemy_hurt_3.mp3", "audio/enemy_hurt_3"},
        {"assets/audio/enemy_run.mp3", "audio/enemy_run"},
        {"assets/audio/enemy_throw_barbs.mp3", "audio/enemy_throw_barbs"},
        {"assets/audio/enemy_throw_silk.mp3", "audio/enemy_throw_silk"},
        {"assets/audio/enemy_throw_sword.mp3", "audio/enemy_throw_sword"},
        {"assets/audio/player_attack_1.mp3", "audio/player_attack_1"},
        {"assets/audio/player_attack_2.mp3", "audio/player_attack_2"},
        {"assets/audio/player_attack_3.mp3", "audio/player_attack_3"},
        {"assets/audio/player_dead.mp3", "audio/player_dead"},
        {"assets/audio/player_hurt.mp3", "audio/player_hurt"},
        {"assets/audio/player_jump.mp3", "audio/player_jump"},
        {"assets/audio/player_land.mp3", "audio/player_land"},
        {"assets/audio/player_roll.mp3", "audio/player_roll"},
        {"assets/audio/player_run.mp3", "audio/player_run"},
    };

    for (const auto& [path, key] : textures) {
        loadTexture(renderer, path, key);
    }

    for (const auto& [dir, count] : enemyAtlases) {
        for (int i = 1; i <= count; ++i) {
            std::string path = "assets/enemy/" + dir + "/" + std::to_string(i) + ".png";
            std::string key = "enemy/" + dir + "/" + std::to_string(i);
            loadTexture(renderer, path, key);
        }
    }

    for (const auto& [path, key] : audios) {
        loadAudio(path, key);
    }
}
#else
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
#endif 

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

#ifdef __EMSCRIPTEN__
void AssetManager::loadEnemyAtlases() {
    static const std::vector<std::pair<std::string, int>> atlasInfo = {
        {"enemy/aim", 9}, {"enemy/barb_break", 3}, {"enemy/barb_loose", 5},
        {"enemy/dash_in_air", 2}, {"enemy/dash_on_floor", 2}, {"enemy/fall", 4},
        {"enemy/idle", 6}, {"enemy/jump", 8}, {"enemy/run", 8}, {"enemy/silk", 9},
        {"enemy/squat", 10}, {"enemy/sword", 3}, {"enemy/throw_barb", 8},
        {"enemy/throw_silk", 17}, {"enemy/throw_sword", 16},
        {"enemy/vfx_dash_in_air", 5}, {"enemy/vfx_dash_on_floor", 6},
    };

    for (const auto& [name, count] : atlasInfo) {
        AtlasAssetInfo info;
        info.name = name;
        info.frameCount = count;
        Atlas* atlas = createAtlasFromDirectory(info);
        if (atlas && atlas->getSize() > 0) {
            atlasPool[name] = atlas;
        } else {
            delete atlas;
        }
    }
}

std::vector<AtlasAssetInfo> AssetManager::discoverAtlases(const std::string&) {
    return {};
}
#else
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
#endif

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
