#ifndef CAMERA_H
#define CAMERA_H

#include "Timer.h"
#include "Vector2.h"

#include <SDL2/SDL.h>

class Camera {
    Vector2 m_position;
    Timer m_timerShake;
    bool m_isShaking = false;
    float m_shakingStrength = 0;
    SDL_Renderer* m_renderer = nullptr;
public:
    Camera(SDL_Renderer* renderer) {
        this->m_renderer = renderer;
        m_timerShake.setOneShot(true);
        m_timerShake.setOnTimeOut([&]() {
            m_isShaking = false;
            reset();
        });
    }

    ~Camera() = default;

    const Vector2& getPosition() const {
        return m_position;
    }
    
    void reset() {
        m_position.x = 0;
        m_position.y = 0;
    }

    void update(float deltaTime) {
        m_timerShake.update(deltaTime);

        if (m_isShaking) {
            m_position.x = (-50 + rand() % 100) / 50.0f * m_shakingStrength;
            m_position.y = (-50 + rand() % 100) / 50.0f * m_shakingStrength;
        }
    }

    void shake(float strength, float duration) {
        m_isShaking = true;
        m_shakingStrength = strength;

        m_timerShake.setDuration(duration);
        m_timerShake.restart();
    }

    void renderTexture(SDL_Texture* texture, const SDL_Rect* rectSrc,
                       const SDL_FRect* rectDst, double angle, const SDL_FPoint* center) const {
        SDL_FRect  rectDstWindow = *rectDst;
        rectDstWindow.x -= m_position.x;
        rectDstWindow.y -= m_position.y;

        SDL_RenderCopyExF(m_renderer, texture, rectSrc, &rectDstWindow, 
                          angle, center, SDL_RendererFlip::SDL_FLIP_NONE);
    }
};

#endif
