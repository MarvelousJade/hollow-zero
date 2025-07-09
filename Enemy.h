#ifndef ENEMY_H
#define ENEMY_H

#include "Animation.h"
#include "Barb.h"
#include "CollisionBox.h"
#include "Sword.h"
#include "Character.h"
#include <vector>

class Enemy : public Character {
    bool m_isThrowingSilk = false;
    bool m_isDashingInAir = false;
    bool m_isDashingOnFloor = false;

    Animation m_animationSilk;
    AnimationGroup m_animationDashInAirVfx;
    AnimationGroup m_animationDashOnFloorVfx;
    Animation* m_currentDashAnimation = nullptr;

    std::vector<Barb*> m_barbList;
    std::vector<Sword*> m_swordList;
    CollisionBox* m_collisionBoxSilk = nullptr;

public:
    Enemy();
    ~Enemy();

    void onUpdate(float deltaTime) override;
    void render(const Camera& camera) override;

    void onHurt() override;

    void setFacingLeft(bool flag) {
        p_isFacingLeft = flag;
    }

    bool getFacingLeft() const {
        return p_isFacingLeft;
    }

    void setDashingInAir(bool flag) {
        m_isDashingInAir = flag;
    }

    bool getDashingInAir() const {
        return m_isDashingInAir;
    }


    void setDashingOnFloor(bool flag) {
        m_isDashingOnFloor = flag;
    }

    bool getDashingOnFloor() const {
        return m_isDashingOnFloor;
    }


    void setThorwingSilk(bool flag) {
        m_isThrowingSilk = flag;
        m_collisionBoxSilk->setEnabled(flag);
    }

    bool getThorwingSilk() const {
        return m_isThrowingSilk;
    }

    void throwBarbs();
    void throwSword();

    void onDash();
    void onThrowSilk();
};

#endif
