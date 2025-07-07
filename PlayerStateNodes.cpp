#include "CollisionBox.h"
#include "Player.h"
#include "CharacterManager.h"
#include "PlayerStateNodes.h"
#include "AssetManager.h"
#include "Vector2.h"
#include "utils.h"

#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include <random>

PlayerAttackState::PlayerAttackState() {
    m_timer.setDuration(0.3f);
    m_timer.setOneShot(true);
    m_timer.setOnTimeOut(
        [&]() {
            Player* player = (Player*)CharacterManager::instance()->getPlayer(); 
            player->setAttacking(false);
        }
    );
}

void PlayerAttackState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("player/attack");

    Player* player = (Player*)CharacterManager::instance()->getPlayer(); 
    player->getHitBox()->setEnabled(true);
    player->setAttacking(true);
    updateHitBoxPosition();
    player->onAttack();
    m_timer.restart();

    switch (randomInt(1, 3)) {
    case 1:
        playAudio("audio/player_attack_1", false);
        break;
    case 2:
        playAudio("audio/player_attack_2", false);
        break;
    case 3:
        playAudio("audio/player_attack_3", false);
        break;
    }
}

void PlayerAttackState::onUpdate(float deltaTime) {
    m_timer.update(deltaTime);
    updateHitBoxPosition();

    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());

    if (player->getHp() <= 0) {
        player ->switchState("dead");
    } else if (!player->getAttacking()) {
        if (player->getVelocity().y > 0) {
            player->switchState("fall");
        } else if (player->getMoveAxis() ==  0) {
            player->switchState("idle");
        } else if (player->isOnFloor() && player->getMoveAxis() !=  0) {
            player->switchState("run");
        }
    }
}

void PlayerAttackState::onExit() {
    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());
    player->getHitBox()->setEnabled(false); 
    player->setAttacking(false); 
}

void PlayerAttackState::updateHitBoxPosition() {
    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());
    Vector2 centerPosition = player->getLogicCenter();
    CollisionBox* hitBox = player->getHitBox();
    const Vector2& hitBoxSize = hitBox->getSize(); 
    Vector2 hitBoxPosition;
    switch (player->getAttackDirection()) {
    case Player::AttackDirection::Up:
        hitBoxPosition = { centerPosition.x, centerPosition.y - hitBoxSize.y / 2};
        break;
    case Player::AttackDirection::Down:
        hitBoxPosition = { centerPosition.x, centerPosition.y + hitBoxSize.y / 2};
        break;
    case Player::AttackDirection::Left:
        hitBoxPosition = { centerPosition.x - hitBoxSize.x / 2, centerPosition.y};
        break;
    case Player::AttackDirection::Right:
        hitBoxPosition = { centerPosition.x + hitBoxSize.x / 2, centerPosition.y};
        break;
    }
    hitBox->setPosition(hitBoxPosition);
}

PlayerDeadState::PlayerDeadState() {
    m_timer.setDuration(2.0f);
    m_timer.setOneShot(true);
    m_timer.setOnTimeOut(
        [&]() {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "No...", "Mission Failes", NULL);
            exit(0);
        }
    );
}

void PlayerDeadState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("dead");

    playAudio("audio/player_dead", false);
}

void PlayerDeadState::onUpdate(float deltaTime) {
    m_timer.update(deltaTime);
}

void PlayerFallState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("fall");
}

void PlayerFallState::onUpdate(float deltaTime) {
    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());

    if (player->getHp() <= 0) {
        player->switchState("dead");
    } else if (player->isOnFloor()) {
        player->switchState("idle");
        player->onLanding();

        playAudio("audio/player_land", false);
    } else if (player->canAttack()) { // Not sure if this should be getAttacking() or not
        player->switchState("attack"); 
    }
}

void PlayerIdleState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("idle");
}

void PlayerIdleState::onUpdate(float deltaTime) {
    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());

    if (player->getHp() <= 0) {
        player->switchState("dead");
    } else if (player->canAttack()) { // Not sure if this should be getAttacking() or not
        player->switchState("attack"); 
    } else if (player->getVelocity().y > 0) {
        player->switchState("fall");
    } else if (player->canJump()) {
        player->switchState("jump");
    } else if (player->canRoll()) {
        player->switchState("roll");
    } else if (player->isOnFloor() && player->getMoveAxis() !=0) {
        player->switchState("run");
    }
}


void PlayerJumpState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("jump");

    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());
    player->onJump();

    playAudio("audio/player_jump", false);
}

void PlayerJumpState::onUpdate(float deltaTime) {
    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());

    if (player->getHp() <= 0) {
        player->switchState("dead");
    } else if (player->getVelocity().y > 0) {
        player->switchState("fall");
    } else if (player->canAttack()) { // Not sure if this should be getAttacking() or not
        player->switchState("attack"); 
    }
}

PlayerRollState::PlayerRollState() {
    m_timer.setDuration(0.35f);
    m_timer.setOneShot(true);
    m_timer.setOnTimeOut(
        [&]() {
            Player* player = (Player*)CharacterManager::instance()->getPlayer(); 
            player->setRolling(false);
        }
    );
}

void PlayerRollState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("player/roll");

    Player* player = (Player*)CharacterManager::instance()->getPlayer(); 
    player->getHurtBox()->setEnabled(false);
    player->setRolling(true);
    player->onRoll();
    m_timer.restart();

    playAudio("audio/player_roll", false);
}

void PlayerRollState::onUpdate(float deltaTime) {
    m_timer.update(deltaTime);

    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());

    if (!player->getRolling()) {
        if (player->getMoveAxis() !=  0) {
            player->switchState("run");
        } else if (player->canJump()) {
            player->switchState("jump");
        } else {
            player->switchState("idle");
        }
    }
}

void PlayerRollState::onExit() {
    CharacterManager::instance()->getPlayer()->getHurtBox()->setEnabled(true);
}

void PlayerRunState::onEnter() {
    CharacterManager::instance()->getPlayer()->setAnimation("run");

    playAudio("audio/player_run", true);
}

void PlayerRunState::onUpdate(float deltaTime) {
    Player* player = dynamic_cast<Player*>(CharacterManager::instance()->getPlayer());

    if (player->getHp() <= 0) {
        player->switchState("dead");
    } else if (player->getMoveAxis() ==0) {
        player->switchState("idle");
    } else if (player->canJump()) {
        player->switchState("jump");
    } else if (player->canAttack()) { // Not sure if this should be getAttacking() or not
        player->switchState("attack"); 
    } else if (player->canRoll()) {
        player->switchState("roll");
    }
}

void PlayerRunState::onExit() {
    Mix_HaltMusic();
}
