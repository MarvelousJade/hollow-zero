#ifndef BARB_H
#define BARB_H

#include "Animation.h"
#include "CollisionBox.h"

class Barb {
public:
    Barb();
    ~Barb();

    void update(float deltaTime);
    void render(const Camera &camera);

    void setPosition(const Vector2& position) {
        m_basePosition = position; 
        m_currentPosition = position; 
    }

    bool checkIsValid() const {
        return m_isValid;
    }

private:
    enum class Stage {
        Idle,
        Aim,
        Dash,
        Break
    };

    const float SPEED_DASH = 1500.0f;

    Timer m_timerIdle;
    Timer m_timerAim;
    int m_diffPeriod = 0;
    bool m_isValid = true;
    float m_totalDeltaTime = 0;

    Vector2 m_basePosition;
    Vector2 m_currentPosition;
    Vector2 m_veclocity;

    Animation m_animationLoose;
    Animation m_animationBreak;
    Animation* m_currentAnimation = nullptr;

    Stage stage = Stage::Idle;
    CollisionBox* m_collisionBox = nullptr;

    void onBreak();
};

#endif
