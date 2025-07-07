#ifndef PLAYER_STATE_NODES_H
#define PLAYER_STATE_NODES_H

#include "Timer.h"
#include "StateNode.h"

class PlayerAttackState : public StateNode {
public: 
    PlayerAttackState();
    ~PlayerAttackState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;
    void onExit() override;

private:
    Timer m_timer;

    void updateHitBoxPosition();
};

class PlayerDeadState : public StateNode {
public: 
    PlayerDeadState();
    ~PlayerDeadState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;

private:
    Timer m_timer;
};

class PlayerFallState : public StateNode {
public: 
    PlayerFallState() = default;
    ~PlayerFallState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;
};

class PlayerIdleState : public StateNode {
public: 
    PlayerIdleState() = default;
    ~PlayerIdleState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;
};


class PlayerJumpState : public StateNode {
public: 
    PlayerJumpState() = default;
    ~PlayerJumpState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;
};

class PlayerRollState : public StateNode {
public: 
    PlayerRollState();
    ~PlayerRollState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;
    void onExit() override;

private:
    Timer m_timer;
};

class PlayerRunState : public StateNode {
public: 
    PlayerRunState();
    ~PlayerRunState() = default;

    void onEnter() override;
    void onUpdate(float deltaTime) override;
    void onExit() override;
};

#endif
