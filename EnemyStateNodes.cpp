#include "Enemy.h"
#include "CharacterManager.h"
#include "Utils.h"
#include "Vector2.h"
#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include "EnemyStateNodes.h"

EnemyAimState::EnemyAimState() {
    m_timer.setOneShot(true);
    m_timer.setDuration(0.5f);
    m_timer.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
            enemy->setGravityEnabled(true);
            enemy->switchState("dashInAir");
        }
    );
}

void EnemyAimState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("aim");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setGravityEnabled(false);
    enemy->setVelocity({ 0, 0 });
    m_timer.restart();
}

void EnemyAimState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timer.update(deltaTime);

    if (enemy->getHp() <= 0) enemy->switchState("dead");
}

void EnemyDashInAirState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("dashInAir");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    const Character* player = CharacterManager::instance()->getPlayer();
    Vector2 targetPosition = { player->getPosition().x, player->getFloorY() };
    enemy->setVelocity((targetPosition - enemy->getPosition()).normalize() * SPEED_DASH);
    enemy->setDashingInAir(true);
    enemy->setGravityEnabled(false);
    enemy->onDash();

    playAudio("audio/enemy_dash", false);
}

void EnemyDashInAirState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    } else if (enemy->isOnFloor()) {
        enemy->switchState("idle");
    }
}

void EnemyDashInAirState::onExit() {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    enemy->setGravityEnabled(true);
    enemy->setDashingInAir(false);
}


EnemyDashOnFloorState::EnemyDashOnFloorState() {
    m_timer.setOneShot(true);
    m_timer.setDuration(0.5f);
    m_timer.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
            enemy->setDashingOnFloor(false);
        }
    );
}

void EnemyDashOnFloorState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("dashOnFloor");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setVelocity({ enemy->getFacingLeft() ?  -SPEED_DASH : SPEED_DASH, 0 });
    enemy->setDashingOnFloor(true);
    enemy->onDash();
    m_timer.restart();

    playAudio("audio/enemy_dash", false);
}

void EnemyDashOnFloorState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timer.update(deltaTime);

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    } else if (!enemy->getDashingOnFloor()) {
        enemy->switchState("idle");
    }
}

void EnemyDeadState::onEnter() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Good, this should work", "Mission Succeed", NULL);
    exit(0);
}

void EnemyFallState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("fall");
}

void EnemyFallState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    } else if (enemy->isOnFloor()) {
        enemy->switchState("idle");
    }
}

EnemyIdleState::EnemyIdleState() {
    m_timer.setOneShot(true);
    m_timer.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

            int randomNum = randomInt(0, 100);
            if (enemy->getHp() > 5) {
                if (randomNum <= 25) {
                    if (!enemy->isOnFloor()) {
                        enemy->switchState("fall");
                    } else {
                        enemy->switchState("jump");
                    }
                } else if (randomNum <= 50) {
                    if (!enemy->isOnFloor()) {
                        enemy->switchState("fall");
                    } else {
                        enemy->switchState("run");
                    }
                } else if (randomNum <= 80) {
                    enemy->switchState("squat");
                } else if (randomNum <= 90) {
                    enemy->switchState("throwSilk");
                } else {
                    enemy->switchState("throwSword");
                }
            } else {
                if (randomNum <= 25) {
                    if (!enemy->isOnFloor()) {
                        enemy->switchState("fall");
                    } else {
                        enemy->switchState("jump");
                    }
                } else if (randomNum <= 60) {
                    enemy->switchState("throwSword");
                } else if (randomNum <= 70) {
                    enemy->switchState("throwSilk");
                } else if (randomNum <= 90) {
                    enemy->switchState("throwBarb");
                } else {
                    enemy->switchState("squat");
                }
            }
        }
    );
}

void EnemyIdleState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("idle");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setVelocity({ 0, 0 });

    float duration =0;
    if(enemy->getHp() > 5) {
        duration = randomInt(0, 2) * 0.25f; // 0.0s to 0.5s
    } else {
        duration = randomInt(0, 1) * 0.25f; // 0.0s to 0.25s
    }

    m_timer.setDuration(duration);
    m_timer.restart();
}

void EnemyIdleState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timer.update(deltaTime);

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    } else if (enemy->getVelocity().y > 0) {
        enemy->switchState("fall");
    }
}

void EnemyIdleState::onExit() {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::instance()->getPlayer()->getPosition().x);
}

void EnemyJumpState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("jump");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setVelocity({ 0, -SPEED_JUMP });
}

void EnemyJumpState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    } else if (enemy->getVelocity().y > 0) {
        int randomNum = randomInt(0, 100);
        if (enemy->getHp() > 5) {
            if (randomNum <= 50) {
                enemy->switchState("aim");
            } else if (randomNum <= 80) {
                enemy->switchState("fall");
            } else {
                enemy->switchState("throwSilk");
            } 
        } else {
            if (randomNum <= 50) {
                enemy->switchState("throwSilk");
            } else if (randomNum <= 80) {
                enemy->switchState("fall");
            } else {
                enemy->switchState("aim");
            }
        }
    }
}

void EnemyRunState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("run");

    m_audioChannel = playAudio("audio/enemyRun", true);
}

void EnemyRunState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    const Vector2& enemyPosition = enemy->getPosition();
    const Vector2& playerPosition = CharacterManager::instance()->getPlayer()->getPosition();
    enemy->setVelocity({ enemyPosition.x < playerPosition.x ? SPEED_RUN : -SPEED_RUN, 0 });

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    } else if (abs(enemyPosition.x - playerPosition.x) <= MIN_DISTANCE) {
        int randomNum = randomInt(0, 100);
        if (enemy->getHp() > 5) {
            if (randomNum <= 75) {
                enemy->switchState("squat");
            } else {
                enemy->switchState("throwSilk");
            } 
        } else {
            if (randomNum <= 75) {
                enemy->switchState("throwSilk");
            } else {
                enemy->switchState("squat");
            }
        }

        Mix_HaltChannel(m_audioChannel);
    }
}


void EnemyRunState::onExit() {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setVelocity({ 0, 0 });
}

EnemySquatState::EnemySquatState() {
    m_timer.setOneShot(true);
    m_timer.setDuration(0.5f);
    m_timer.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
            enemy->switchState("dashOnFloor");
        }
    );
}

void EnemySquatState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("squat");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::instance()->getPlayer()->getPosition().x);
    m_timer.restart();
}

void EnemySquatState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timer.update(deltaTime);

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    }
}

EnemyThrowBarbState::EnemyThrowBarbState() {
    m_timer.setOneShot(true);
    m_timer.setDuration(0.8f);
    m_timer.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
            enemy->throwBarbs();
            enemy->switchState("idle");
        }
    );
}

void EnemyThrowBarbState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("throwBarb");

    m_timer.restart();

    playAudio("audio/enemy_throw_barbs", false);
}

void EnemyThrowBarbState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timer.update(deltaTime);

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    }
}

EnemyThrowSilkState::EnemyThrowSilkState() {
    m_timer.setOneShot(true);
    m_timer.setDuration(0.9f);
    m_timer.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
            enemy->setGravityEnabled(true);
            enemy->setThorwingSilk(false);
            if (!enemy->isOnFloor() && enemy->getHp() > 5 && randomInt(0, 100) <= 25) {
                enemy->switchState("aim");
            } else if (!enemy->isOnFloor()) {
                enemy->switchState("fall");
            } else {
                enemy->switchState("idle");
            }
        }
    );
}

void EnemyThrowSilkState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("throwSilk");

    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
    enemy->setGravityEnabled(false);
    enemy->setThorwingSilk(true);
    enemy->setVelocity({ 0, 0 });
    enemy->onThrowSilk();
    m_timer.restart();

    playAudio("audio/enemy_throw_silk", false);
}

void EnemyThrowSilkState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timer.update(deltaTime);

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    }
}


EnemyThrowSwordState::EnemyThrowSwordState() {
    m_timerThrow.setOneShot(true);
    m_timerThrow.setDuration(0.65f);
    m_timerThrow.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());
            enemy->throwSword();

            playAudio("audio/enemy_throw_sword", false);
        }
    );

    m_timerSwitch.setDuration(1.0f);
    m_timerSwitch.setOneShot(true);
    m_timerSwitch.setOnTimeOut(
        [&]() {
            Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

            int randomNum = randomInt(0, 100);
            if (enemy->getHp() > 5) {
                if (randomNum <= 50) {
                    enemy->switchState("squat");
                } else if (randomNum <= 80){
                    enemy->switchState("jump");
                } else {
                    enemy->switchState("idle");
                } 
            } else {
                if (randomNum <= 50) {
                    enemy->switchState("jump");
                } else if (randomNum <= 80){
                    enemy->switchState("throwSilk");
                } else {
                    enemy->switchState("idle");
                } 
            }
        }
    );
}


void EnemyThrowSwordState::onEnter() {
    CharacterManager::instance()->getEnemy()->setAnimation("throwSilk");
    m_timerThrow.restart();
    m_timerSwitch.restart();
}

void EnemyThrowSwordState::onUpdate(float deltaTime) {
    Enemy* enemy = dynamic_cast<Enemy*>(CharacterManager::instance()->getEnemy());

    m_timerThrow.update(deltaTime);
    m_timerSwitch.update(deltaTime);

    if (enemy->getHp() <= 0) {
        enemy->switchState("dead");
    }
}
