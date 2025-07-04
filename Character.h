#ifndef CHARACTER_H
#define CHARACTER_H

#include "Vector2.h"
#include "Animation.h"
#include "CollisionBox.h"
#include "StateMachine.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_events.h>
#include <string>
#include <unordered_map>

class Character {
public:
    Character();
    ~Character();

    void decreaseHp();

    int getHp() const {
        return p_hp;
    }

    void setPosition(const Vector2& position) {
        p_position = position;
    }

    const Vector2& getPosition() const {
        return p_position;
    }

    void setVelocity(const Vector2& velocity) {
        p_velocity = velocity;
    }

    const Vector2& getVelocity() const {
        return p_velocity;
    }

    Vector2 getLogicCenter() const {
        return Vector2(p_position.x, p_position.y - p_logicHeight / 2);
    }

    void setGravityEnabled(bool flag) {
        p_shouldEnableGravity = flag;
    }

    CollisionBox* getHitBox() {
        return p_hitBox;
    }

    CollisionBox* getHurtBox() {
        return p_hurtBox;
    }

    bool isOnFloor() const {
        return p_position.y >= FLOOR_Y; // >=
    }

    float getFloorY() const {
            return FLOOR_Y;
    }

    void makeInvulnerable() {
        p_isInvulnerable = true;
        p_timerInvulnerableStatus.restart();
    }

    virtual void onInput(const SDL_Event& event);
    virtual void onUpdate(float deltaTime);
    virtual void render(const Camera& camera);

    virtual void onHurt();

    void switchState(const std::string& id);
    void setAnimation(const std::string& id);

protected:
    struct AnimationGroup {
        Animation left;
        Animation right;
    };

    const float FLOOR_Y = 620;
    const float GRAVITY_Y = 980;

    int p_hp = 10;
    Vector2 p_position;
    Vector2 p_velocity;
    float p_logicHeight = 0;
    bool p_isFacingLeft = true;
    StateMachine p_stateMachine;
    bool p_shouldEnableGravity = true;
    bool p_isInvulnerable = false;
    Timer p_timerInvulnerableBlink;
    Timer p_timerInvulnerableStatus;
    bool p_isBlinkInvisible = false;
    CollisionBox* p_hitBox = nullptr;
    CollisionBox* p_hurtBox = nullptr;
    AnimationGroup* p_currentAnimation = nullptr;
    std::unordered_map< std::string, AnimationGroup> p_animationPool;

};

#endif
