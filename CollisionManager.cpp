#include "CollisionManager.h"

#include <SDL2/SDL.h>

#include <algorithm>

CollisionManager* CollisionManager::s_collisionManager = nullptr;

CollisionManager::CollisionManager() = default;

CollisionManager::~CollisionManager() = default;

CollisionManager* CollisionManager::instance() {
    if (!s_collisionManager) s_collisionManager = new CollisionManager();

    return s_collisionManager;
}

CollisionBox* CollisionManager::createCollisionBox() {
    CollisionBox* collisionBox = new CollisionBox();

    m_collisionBoxList.push_back(collisionBox);

    return collisionBox;
}

void CollisionManager::destroyCollisionBox(CollisionBox* collisionBox) {
    m_collisionBoxList.erase(
        std::remove(m_collisionBoxList.begin(), m_collisionBoxList.end(), collisionBox),
        m_collisionBoxList.end()
    );

    delete collisionBox;
}

void CollisionManager::handleCollision() {
    for (CollisionBox* collisionBoxSrc : m_collisionBoxList) {
        if (!collisionBoxSrc->m_isEnabled 
            || collisionBoxSrc->m_layerDst == CollisionLayer::None) 
            continue;

        for (CollisionBox* collisionBoxDst : m_collisionBoxList) {

            if (!collisionBoxDst->m_isEnabled 
                || collisionBoxSrc == collisionBoxDst 
                ||collisionBoxSrc->m_layerDst != collisionBoxDst->m_layerSrc) 
                continue;

            bool isCollidingX = (
                fmax(collisionBoxSrc->m_position.x + collisionBoxSrc->m_size.x / 2, collisionBoxDst->m_position.x + collisionBoxDst->m_size.x / 2)
                - fmin(collisionBoxSrc->m_position.x - collisionBoxSrc->m_size.x / 2, collisionBoxDst->m_position.x - collisionBoxDst->m_size.x / 2)
                <= collisionBoxSrc->m_size.x + collisionBoxDst->m_size.x
            );

            bool isCollidingY = (
                fmax(collisionBoxSrc->m_position.y + collisionBoxSrc->m_size.y / 2, collisionBoxDst->m_position.y + collisionBoxDst->m_size.y / 2)
                - fmin(collisionBoxSrc->m_position.y - collisionBoxSrc->m_size.y / 2, collisionBoxDst->m_position.y - collisionBoxDst->m_size.y / 2)
                <= collisionBoxSrc->m_size.y + collisionBoxDst->m_size.y / 2
            );

            if (isCollidingX && isCollidingY && collisionBoxDst->m_onCollide) {
                collisionBoxDst->m_onCollide();
            }
        }
    }
}

void CollisionManager::debugRender(const Camera& camera){
    for (CollisionBox* collisionBox : m_collisionBoxList) {
        SDL_Rect rect;
        rect.x = collisionBox->m_position.x - collisionBox->m_size.x / 2;
        rect.y = collisionBox->m_position.y - collisionBox->m_size.y / 2;
        rect.w = collisionBox->m_size.x;
        rect.h = collisionBox->m_size.y;

        SDL_Renderer* renderer = camera.getRenderer();

        if (collisionBox->m_isEnabled) {
            SDL_SetRenderDrawColor(renderer, 255, 195, 195, 255);  // Light red (enabled)
        } else {
            SDL_SetRenderDrawColor(renderer, 115, 115, 175, 255);  // Purple-gray (disabled)
        }

        SDL_RenderDrawRect(renderer, &rect);
    }
}
