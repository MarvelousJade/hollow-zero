#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "Character.h"

class Player : public Character {
public:
    enum class AttackDirection {
        Up, Down, Left, Right
    };

public:
    Player();
    ~Player();

    void onInput(const SDL_Event& event) override;
    void onUpdate(float deltaTime) override;
    void render(const Camera& camera) override;

    void onHurt() override;

    void setRolling(bool flag) {
        m_isRolling = flag;
    }

    bool getRolling() const {
        return m_isRolling;
    }
 
    bool canRoll() const {
        return m_isRollOnCooldown && !m_isRolling && m_isRollKeyDown;
    }

    void setAttacking(bool flag) {
        m_isAttacking = flag;
    }

    bool getAttacking() const {
        return m_isAttacking;
    }
 
    bool canAttack() const {
        return m_isAttackOnCooldown && !m_isAttacking && m_isAttackKeyDown;
    }

    bool canJump() const {
        return isOnFloor() && m_isJumpKeyDown;
    }

    int getMoveAxis() const {
        return m_isRightKeyDown - m_isLeftKeyDown;
    }

    AttackDirection getAttackDirection() const {
        return m_attackDirection;
    }

    void onJump();
    void onLanding();
    void onRoll();
    void onAttack();

private:
    const float CD_ROLL = 0.75f; 
    const float CD_ATTACK = 0.5f; 
    const float SPEED_RUN = 300.0f; 
    const float SPEED_JUMP = 700.0f; 
    const float SPEED_ROLL = 800.0f; 

    Timer m_timerRollCooldown;
    bool m_isRolling  = false;
    bool m_isRollOnCooldown  = true;

    Timer m_timerAttackCooldown;
    bool m_isAttacking  = false;
    bool m_isAttackOnCooldown  = true;

    bool m_isLeftKeyDown  = false;
    bool m_isRightKeyDown  = false;
    bool m_isUpKeyDown  = false;
    bool m_isDownKeyDown  = false;
    bool m_isJumpKeyDown  = false;
    bool m_isRollKeyDown  = false;
    bool m_isAttackKeyDown  = false;

    Animation m_animationSlashUp;
    Animation m_animationSlashDown;
    Animation m_animationSlashLeft;
    Animation m_animationSlashRight;
    AttackDirection m_attackDirection = AttackDirection::Right;
    Animation* m_currentSlashAnimation = nullptr;

    bool m_isJumpVfxVisible = false;
    Animation m_animationJumpVfx;

    bool m_isLandingVfxVisible = false;
    Animation m_animationLandingVfx;

    void updateAttackDirection(int x, int y);

};

#endif
