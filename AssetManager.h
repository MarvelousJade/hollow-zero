#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

#include "Atlas.h"

struct AtlasAssetInfo; 

class AssetManager {
    AssetManager();
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    static AssetManager* s_assetManager;

    std::unordered_map<std::string, Mix_Chunk*> audioPool;
    std::unordered_map<std::string, SDL_Texture*> texturePool;
    std::unordered_map<std::string, Atlas*> atlasPool;

    // Helper methods for loading
    void loadTexturesAndAudio(SDL_Renderer* renderer, const std::filesystem::path& assetsPath);
    void loadTexture(SDL_Renderer* renderer, const std::filesystem::path& filePath, const std::string& key);
    void loadAudio(const std::filesystem::path& filePath, const std::string& key);
    
    void loadAtlases();
    void loadEnemyAtlases();
    
    std::vector<AtlasAssetInfo> discoverAtlases(const std::string& subdir);
    Atlas* createAtlasFromDirectory(const AtlasAssetInfo& info);

public:
    static AssetManager* instance();

    void load(SDL_Renderer* renderer);

    Mix_Chunk* findAudio(const std::string& audioName);
    SDL_Texture* findTexture(const std::string& textureName);
    Atlas* findAtlas(const std::string& atlasName);

    Atlas* createAtlas(const std::vector<std::string>& textureNames);
};

#endif
