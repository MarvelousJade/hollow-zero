#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "Atlas.h"

struct AtlasAssetInfo; 

class AssetManager {
    AssetManager();
    ~AssetManager();

    static AssetManager* assetManager;
    std::unordered_map<std::string, Mix_Chunk*> audioPool;
    std::unordered_map<std::string, SDL_Texture*> texturePool;
    std::unordered_map<std::string, Atlas*> atlasPool;

public:
    static AssetManager* instance();

    void load(SDL_Renderer* renderer);
    std::vector<AtlasAssetInfo> discoverEnemyAtlases();

    Mix_Chunk* findAudio(const std::string& audioName);
    SDL_Texture* findTexture(const std::string& textureName);
    Atlas* findAtlas(const std::string& atlasName);

    Atlas* createAtlas(const std::vector<std::string>& textureNames);
    Atlas* createAtlasByPattern(const std::string& pattern, int count);
    void loadEnemyAtlases();

    void flipTexture(SDL_Texture* srcTexture, SDL_Texture* dstTexture, int horizontalFrames);
    void flipTexture(const std::string& srcId, const std::string& dstId, int horizontalFrames);
    void flipAtlas(const std::string& srcId, const std::string& dstId);
};

#endif
