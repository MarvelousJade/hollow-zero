#ifndef ANIMATION_H
#define ANIMATION_H

#include "Timer.h"
#include "Atlas.h"
#include "Camera.h"
#include "Vector2.h"

#include <SDL2/SDL.h>

#include <type_traits>
#include <vector>
#include <functional>

class Animation {
public:
    enum class AnchorMode {
        Centered,
        BottomCentered
    };

private:
    struct Frame {
        SDL_Rect m_rectSrc;
        SDL_Texture* m_texture = nullptr;

        Frame() = default; 
        Frame(SDL_Texture* texture, const SDL_Rect& rectSrc) 
            : m_texture(texture), m_rectSrc(rectSrc) {}; 

        ~Frame() = default; 
    };

    Vector2 m_position;
    double m_angle = 0;
    SDL_FPoint m_center = { 0 };

    Timer m_timer;
    bool m_isLoop = true;
    size_t m_idxFrame = 0;
    std::vector<Frame> m_frameList;
    std::function<void()> m_onFinished;
    AnchorMode m_anchorMode = AnchorMode::Centered;

public:
    Animation() {
        m_timer.setOneShot(false);
        m_timer.setOnTimeOut(
            [&]() {
                m_idxFrame++;
                if (m_idxFrame >= m_frameList.size()) {
                    m_idxFrame = m_isLoop ? 0 : m_frameList.size() - 1;
                    if (!m_isLoop && m_onFinished) {
                        m_onFinished();
                    }
                }
            }
        );
    }

    ~Animation() = default;

    void reset() {
        m_timer.restart();
        m_idxFrame = 0;
    }

    void setAnchorMode(AnchorMode mode) {
        m_anchorMode  = mode;
    }

    void setPosition(const Vector2& position) {
        m_position = position;
    }

    void setRotation(double angle) {
        m_angle = angle;
    }

    void setCenter(const SDL_FPoint& center) {
        m_center = center;
    }

    void setLoop(bool isLoop) {
        m_isLoop = isLoop;
    }

    void setInterval(float interval) {
        m_timer.setDuration(interval);
    }

    void setOnFinished(std::function<void()> onFinished) {
        m_onFinished = onFinished;
    }

    void addFrame(SDL_Texture* texture, int horizontalFrames) {
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        int frameWidth = width / horizontalFrames;

        for (int i = 0; i < horizontalFrames; i++) {
            SDL_Rect rectSrc;
            rectSrc.x = i * frameWidth, rectSrc.y = 0;
            rectSrc.w = frameWidth, rectSrc.h = height;

            m_frameList.emplace_back(texture, rectSrc);
        }
    }

    void addFrame(Atlas* atlas) {
        for (int i =0; i < atlas->getSize(); i++) {
            SDL_Texture* texture = atlas->getTexture(i);

            int width, height;
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

            SDL_Rect rectSrc;
            rectSrc.x = 0, rectSrc.y = 0;
            rectSrc.w = width, rectSrc.h = height;

            m_frameList.emplace_back(texture, rectSrc);
        }
    }

    void update(float deltaTime) {
        m_timer.update(deltaTime);
    }

    void render(const Camera& camera) const {
        const Frame& frame = m_frameList[m_idxFrame];
        const Vector2& cameraPos = camera.getPosition();

        SDL_FRect rectDst;
        rectDst.x = m_position.x - frame.m_rectSrc.w / 2.0f;
        if (m_anchorMode == AnchorMode::BottomCentered) {
            rectDst.y = m_position.y - frame.m_rectSrc.h;
        } else {
            rectDst.y = m_position.y - frame.m_rectSrc.h / 2.0f;
        }
        rectDst.w = (float)frame.m_rectSrc.w; 
        rectDst.h = (float)frame.m_rectSrc.h;

        camera.renderTexture(frame.m_texture, &frame.m_rectSrc, &rectDst, m_angle, &m_center);
    }
};

#endif
