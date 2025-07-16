#ifndef SWORD_H
#define SWORD_H

#include "Animation.h"
#include "CollisionBox.h"
#include "Vector2.h"

class Sword {
    const float SPEED_MOVE = 1250.0f;

    Vector2 m_position;
    Vector2 m_veclocity;
    Animation m_animation;
    bool m_isValid = true;
    CollisionBox* m_collisionBox = nullptr;

public:
    Sword(const Vector2& positoin, bool shouldMoveLeft);
    ~Sword();

    void update(float deltaTime);
    void render(const Camera &camera);

    bool checkIsValid() const {
        return m_isValid;
    }
};

#endif
