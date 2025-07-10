#ifndef ENEMY_STATE_NODES_H
#define ENEMY_STATE_NODES_H

#include "Timer.h"
#include "StateNode.h"

class EnemyAimState : public StateNode {
public:
    EnemyAimState();
    ~EnemyAimState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    Timer m_timer;
};

class EnemyDashInAirState : public StateNode {
public:
    EnemyDashInAirState() = default;
    ~EnemyDashInAirState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;
    void onExit() override;

private:
    Timer m_timer;
    const float SPEED_DASH = 1500.0f;
};

class EnemyDashOnFloorState : public StateNode {
public:
    EnemyDashOnFloorState();
    ~EnemyDashOnFloorState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    Timer m_timer;
    const float SPEED_DASH = 1500.0f;
};

class EnemyDeadState : public StateNode {
public:
    EnemyDeadState() = default;
    ~EnemyDeadState() = default;

    void onEnter() override;
};

class EnemyFallState : public StateNode {
public:
    EnemyFallState() = default;
    ~EnemyFallState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;
};


class EnemyIdleState : public StateNode {
public:
    EnemyIdleState();
    ~EnemyIdleState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;
    void onExit() override;

private:
    Timer m_timer;
};

class EnemyJumpState : public StateNode {
public:
    EnemyJumpState();
    ~EnemyJumpState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    const float SPEED_JUMP = 1000.0f;
};

class EnemyRunState : public StateNode {
public:
    EnemyRunState();
    ~EnemyRunState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;
    void onExit() override;

private:
    const float MIN_DISTANCE = 350.0f;
    const float SPEED_RUN = 500.0f;

    int m_audioChannel = -1;
};

class EnemySquatState : public StateNode {
public:
    EnemySquatState();
    ~EnemySquatState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    Timer m_timer;
};

class EnemyThrowBarbState : public StateNode {
public:
    EnemyThrowBarbState();
    ~EnemyThrowBarbState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    Timer m_timer;
};

class EnemyThrowSilkState : public StateNode {
public:
    EnemyThrowSilkState();
    ~EnemyThrowSilkState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    Timer m_timer;
};

class EnemyThrowSwordState : public StateNode {
public:
    EnemyThrowSwordState();
    ~EnemyThrowSwordState() = default;

    void onEnter() override;
    void onUpdate(float deltaTim) override;

private:
    Timer m_timerThrow;
    Timer m_timerSwitch;
};

#endif
