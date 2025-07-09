#include "BulletTimeManager.h"

#include <SDL2/SDL.h>

BulletTimeManager* BulletTimeManager::s_manager = nullptr;


BulletTimeManager::BulletTimeManager() = default;
BulletTimeManager::~BulletTimeManager() = default;

BulletTimeManager* BulletTimeManager::instance() {
    if (!s_manager) {
        s_manager = new BulletTimeManager();
    }

    return s_manager;
};

void BulletTimeManager::postProcess(const Camera& camera) {
    SDL_Renderer* renderer = camera.getRenderer();
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    Uint8 alpha = lerp(0.0f, 192.0f, m_progress); 

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);     

    SDL_Rect fullScreen = {0, 0, 1280, 720};
    SDL_RenderFillRect(renderer, &fullScreen);
}


void BulletTimeManager::setStatus(Status status) {
    m_status = status;
}

float BulletTimeManager::update(float deltaTime) {
    float deltaProgress = SPEED_PROGRESS * deltaTime;
    m_progress += deltaProgress * (m_status == Status::Entering ? 1 : -1);

    if (m_progress < 0) m_progress = 0; 
    if (m_progress > 1.0f) m_progress = 1.0f; 

    return deltaTime * lerp(1.0f, DESTINATION_DELTA_FACTOR, m_progress);
}

