#include "Player.h"
#include "AssetManager.h"
#include "Utils.h"
#include "PlayerStateNodes.h"
#include "BulletTimeManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <cmath>
#include <iostream>

Player::Player() {
    p_isFacingLeft = false;
    p_position = { 250, 200 };
    p_logicHeight = 120;

    p_hitBox->setSize({ 150, 150 });
    p_hurtBox->setSize({ 40, 80 });

    p_hitBox->setLayerSrc(CollisionLayer::Player);
    p_hitBox->setLayerDst(CollisionLayer::Enemy);

    p_hurtBox->setLayerSrc(CollisionLayer::Player);
    p_hurtBox->setLayerDst(CollisionLayer::None);

    p_hitBox->setEnabled(false);

    p_hitBox->setOnCollide(
        [&]() {
            if (!m_hasPlayedHitAudioThisFrame) {
                int attackSound = randomInt(1, 3);
                playAudio("audio/player_attack_" + std::to_string(attackSound), false);
                m_hasPlayedHitAudioThisFrame = true;
            }
        }
    );

    p_hurtBox->setOnCollide(
        [&]() {
            decreaseHp();
        }
    );

    m_timerRollCooldown.setDuration(CD_ROLL);
    m_timerRollCooldown.setOneShot(true);
    m_timerRollCooldown.setOnTimeOut(
        [&]() {
            m_isRollOnCooldown = true;
        }
    );

    m_timerAttackCooldown.setDuration(CD_ATTACK);
    m_timerAttackCooldown.setOneShot(true);
    m_timerAttackCooldown.setOnTimeOut(
        [&]() {
            m_isAttackOnCooldown = true;
        }
    );

    std::cout << "Setting up player animations..." << std::endl;
    
    // Check critical textures before using them:
    SDL_Texture* attackTex = AssetManager::instance()->findTexture("player/attack");
    SDL_Texture* idleTex = AssetManager::instance()->findTexture("player/idle");
    SDL_Texture* rollTex = AssetManager::instance()->findTexture("player/roll");
    SDL_Texture* runTex = AssetManager::instance()->findTexture("player/run");
    SDL_Texture* attackVfxTex = AssetManager::instance()->findTexture("player/vfx_attack_right");

    std::cout << "Attack texture: " << (attackTex ? "OK" : "NULL") << std::endl;
    std::cout << "Idle texture: " << (idleTex ? "OK" : "NULL") << std::endl;
    std::cout << "Roll texture: " << (rollTex ? "OK" : "NULL") << std::endl;
    std::cout << "Run texture: " << (runTex ? "OK" : "NULL") << std::endl;
    std::cout << "Attack Vfx texture: " << (attackVfxTex ? "OK" : "NULL") << std::endl;
    
    // Test SDL_QueryTexture directly:
    if (attackTex) {
        int w, h;
        int result = SDL_QueryTexture(attackTex, nullptr, nullptr, &w, &h);
        std::cout << "Attack texture query result: " << result << ", size: " << w << "x" << h << std::endl;
    }

    if (!attackTex || !idleTex || !rollTex || !runTex) {
        throw std::runtime_error("Missing critical textures - cannot create Player");
    }

    {
        {
            AnimationGroup& animationAttack = p_animationPool["attack"];

            Animation& animationAttackLeft = animationAttack.left;
            animationAttackLeft.setInterval(0.05f);
            animationAttackLeft.setLoop(false);
            animationAttackLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationAttackLeft.addFrame(AssetManager::instance()->findTexture("player/attack"), 5, true);

            Animation& animationAttackRight = animationAttack.right;
            animationAttackRight.setInterval(0.05f);
            animationAttackRight.setLoop(false);
            animationAttackRight.setAnchorMode(Animation::AnchorMode::BottomCentered);

            animationAttackRight.addFrame(AssetManager::instance()->findTexture("player/attack"), 5);
        }
        {
            AnimationGroup& animationDead = p_animationPool["dead"];

            Animation& animationDeadLeft = animationDead.left;
            animationDeadLeft.setInterval(0.1f);
            animationDeadLeft.setLoop(false);
            animationDeadLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDeadLeft.addFrame(AssetManager::instance()->findTexture("player/dead"), 6, true);

            Animation& animationDeadRight = animationDead.right;
            animationDeadRight.setInterval(0.1f);
            animationDeadRight.setLoop(false);
            animationDeadRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDeadRight.addFrame(AssetManager::instance()->findTexture("player/dead"), 6);
        }
        {
            AnimationGroup& animationFall = p_animationPool["fall"];

            Animation& animationFallLeft = animationFall.left;
            animationFallLeft.setInterval(0.15f);
            animationFallLeft.setLoop(true);
            animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationFallLeft.addFrame(AssetManager::instance()->findTexture("player/fall"), 5, true);

            Animation& animationFallRight = animationFall.right;
            animationFallRight.setInterval(0.15f);
            animationFallRight.setLoop(true);
            animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationFallRight.addFrame(AssetManager::instance()->findTexture("player/fall"), 5);
        }
        {
            AnimationGroup& animationIdle = p_animationPool["idle"];

            Animation& animationIdleLeft = animationIdle.left;
            animationIdleLeft.setInterval(0.15f);
            animationIdleLeft.setLoop(true);
            animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationIdleLeft.addFrame(AssetManager::instance()->findTexture("player/idle"), 5, true);

            Animation& animationIdleRight = animationIdle.right;
            animationIdleRight.setInterval(0.15f);
            animationIdleRight.setLoop(true);
            animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationIdleRight.addFrame(AssetManager::instance()->findTexture("player/idle"), 5);
        }
        {
            AnimationGroup& animationJump = p_animationPool["jump"];

            Animation& animationJumpLeft = animationJump.left;
            animationJumpLeft.setInterval(0.15f);
            animationJumpLeft.setLoop(false);
            animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpLeft.addFrame(AssetManager::instance()->findTexture("player/jump"), 5, true);

            Animation& animationJumpRight = animationJump.right;
            animationJumpRight.setInterval(0.15f);
            animationJumpRight.setLoop(false);
            animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpRight.addFrame(AssetManager::instance()->findTexture("player/jump"), 5);
        }
        {
            AnimationGroup& animationRoll = p_animationPool["roll"];

            Animation& animationRollLeft = animationRoll.left;
            animationRollLeft.setInterval(0.05f);
            animationRollLeft.setLoop(false);
            animationRollLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRollLeft.addFrame(AssetManager::instance()->findTexture("player/roll"), 7, true);

            Animation& animationRollRight = animationRoll.right;
            animationRollRight.setInterval(0.05f);
            animationRollRight.setLoop(false);
            animationRollRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRollRight.addFrame(AssetManager::instance()->findTexture("player/roll"), 7);
        }
        {
            AnimationGroup& animationRun = p_animationPool["run"];

            Animation& animationRunLeft = animationRun.left;
            animationRunLeft.setInterval(0.075f);
            animationRunLeft.setLoop(true);
            animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunLeft.addFrame(AssetManager::instance()->findTexture("player/run"), 10, true);

            Animation& animationRunRight = animationRun.right;
            animationRunRight.setInterval(0.075f);
            animationRunRight.setLoop(true);
            animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunRight.addFrame(AssetManager::instance()->findTexture("player/run"), 10);
        }
    }
    {
        m_animationSlashUp.setInterval(0.07f);
        m_animationSlashUp.setLoop(false);
        m_animationSlashUp.setAnchorMode(Animation::AnchorMode::Centered);
        m_animationSlashUp.addFrame(AssetManager::instance()->findTexture("player/vfx_attack_up"), 5);

        m_animationSlashDown.setInterval(0.07f);
        m_animationSlashDown.setLoop(false);
        m_animationSlashDown.setAnchorMode(Animation::AnchorMode::Centered);
        m_animationSlashDown.addFrame(AssetManager::instance()->findTexture("player/vfx_attack_down"), 5);

        m_animationSlashLeft.setInterval(0.07f);
        m_animationSlashLeft.setLoop(false);
        m_animationSlashLeft.setAnchorMode(Animation::AnchorMode::Centered);
        m_animationSlashLeft.addFrame(AssetManager::instance()->findTexture("player/vfx_attack_left"), 5);

        m_animationSlashRight.setInterval(0.07f);
        m_animationSlashRight.setLoop(false);
        m_animationSlashRight.setAnchorMode(Animation::AnchorMode::Centered);
        m_animationSlashRight.addFrame(AssetManager::instance()->findTexture("player/vfx_attack_right"), 5);

        m_animationJumpVfx.setInterval(0.05f);
        m_animationJumpVfx.setLoop(false);
        m_animationJumpVfx.setAnchorMode(Animation::AnchorMode::BottomCentered);
        m_animationJumpVfx.addFrame(AssetManager::instance()->findTexture("player/vfx_jump"), 5);
        m_animationJumpVfx.setOnFinished([&]() { m_isJumpVfxVisible = false; });

        m_animationLandingVfx.setInterval(0.1f);
        m_animationLandingVfx.setLoop(false);
        m_animationLandingVfx.setAnchorMode(Animation::AnchorMode::BottomCentered);
        m_animationLandingVfx.addFrame(AssetManager::instance()->findTexture("player/vfx_land"), 2);
        m_animationLandingVfx.setOnFinished([&]() { m_isLandingVfxVisible = false; });
    }

    {
        p_stateMachine.registerState("attack", new PlayerAttackState());
        p_stateMachine.registerState("dead", new PlayerDeadState());
        p_stateMachine.registerState("fall", new PlayerFallState());
        p_stateMachine.registerState("idle", new PlayerIdleState());
        p_stateMachine.registerState("jump", new PlayerJumpState());
        p_stateMachine.registerState("roll", new PlayerRollState());
        p_stateMachine.registerState("run", new PlayerRunState());

        p_stateMachine.setEntry("idle");
    }
}

Player::~Player() = default;

void Player::onInput(const SDL_Event& e) {
    if (p_hp <=0) return;

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
    m_isLeftKeyDown = currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT];
    m_isRightKeyDown = currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT];
    m_isUpKeyDown = currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP];
    m_isDownKeyDown = currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN];

    int bulletTimtChannel = -1;

    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_J:
                    m_isAttackKeyDown = true;
                    updateAttackDirection();
                    break;
                case SDL_SCANCODE_K:
                case SDL_SCANCODE_SPACE:
                    m_isJumpKeyDown = true;
                    break;
                case SDL_SCANCODE_L:
                case SDL_SCANCODE_LSHIFT:
                    m_isRollKeyDown = true;
                    break;
                case SDL_SCANCODE_R:
                    // Bullet time
                    if (BulletTimeManager::instance()->getStatus() ==  BulletTimeManager::Status::Entering) {
                        BulletTimeManager::instance()->setStatus(BulletTimeManager::Status::Exiting);
                        if (bulletTimtChannel != -1) Mix_HaltChannel(bulletTimtChannel);
                    } else {
                        bulletTimtChannel = playAudio("audio/bullet_time", false);
                        BulletTimeManager::instance()->setStatus(BulletTimeManager::Status::Entering);
                    }                  
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_J:
                    m_isAttackKeyDown = false;
                    break;
                case SDL_SCANCODE_K:
                case SDL_SCANCODE_SPACE:
                    m_isJumpKeyDown = false;
                    break;
                case SDL_SCANCODE_L:
                case SDL_SCANCODE_LSHIFT:
                    m_isRollKeyDown = false;
                    break;            
                case SDL_SCANCODE_R:
                    // Bullet time
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    m_isAttackKeyDown = true;
                    updateAttackDirection(e.motion.x, e.motion.y);
                    break;
                case SDL_BUTTON_RIGHT:
                    if (BulletTimeManager::instance()->getStatus() ==  BulletTimeManager::Status::Entering) {
                        BulletTimeManager::instance()->setStatus(BulletTimeManager::Status::Exiting);
                        if (bulletTimtChannel != -1) Mix_HaltChannel(bulletTimtChannel);
                    } else {
                        bulletTimtChannel = playAudio("audio/bullet_time", false);
                        BulletTimeManager::instance()->setStatus(BulletTimeManager::Status::Entering);
                    }
                    break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    m_isAttackKeyDown = false;
                    break;
            }
            break; 
    }
}

void Player::onUpdate(float deltaTime) {
    // Reset hit audio flag each frame
    m_hasPlayedHitAudioThisFrame = false;
    
    if (p_hp > 0 && !m_isRolling) {
        p_velocity.x = getMoveAxis() * SPEED_RUN;
    }

    if (getMoveAxis() != 0) {
        p_isFacingLeft = (getMoveAxis() < 0);
    }

    m_timerRollCooldown.update(deltaTime);
    m_timerAttackCooldown.update(deltaTime);

    m_animationJumpVfx.update(deltaTime);
    m_animationLandingVfx.update(deltaTime);

    if (m_isAttacking) {
        m_currentSlashAnimation->setPosition(getLogicCenter());
        m_currentSlashAnimation->update(deltaTime);
    }

    Character::onUpdate(deltaTime);
}

void Player::render(const Camera& camera) {
    if (m_isJumpVfxVisible) {
        m_animationJumpVfx.render(camera);
    }

    if (m_isLandingVfxVisible) {
        m_animationLandingVfx.render(camera);
    }

    Character::render(camera);

    if (m_isAttacking && m_currentSlashAnimation) {
        m_currentSlashAnimation->render(camera);
    }
}

void Player::onHurt() {
    playAudio("audio/player_hurt", false); 
}

void Player::onJump() {
    p_velocity.y -= SPEED_JUMP;
    m_isJumpVfxVisible = true;
    m_animationJumpVfx.setPosition(p_position);
    m_animationJumpVfx.reset();
}

void Player::onLanding() {
    m_isLandingVfxVisible = true;
    m_animationLandingVfx.setPosition(p_position);
    m_animationLandingVfx.reset();
}

void Player::onRoll() {
    m_timerRollCooldown.restart();
    m_isRollOnCooldown = false;
    p_velocity.x = p_isFacingLeft ? -SPEED_ROLL : SPEED_ROLL;
}

void Player::onAttack() {
    m_timerAttackCooldown.restart();
    m_isAttackOnCooldown = false;
    switch(m_attackDirection) {
    case Player::AttackDirection::Up:
        m_currentSlashAnimation = &m_animationSlashUp;
        break;
    case Player::AttackDirection::Down:
        m_currentSlashAnimation = &m_animationSlashDown;
        break;
    case Player::AttackDirection::Left:
        m_currentSlashAnimation = &m_animationSlashLeft;
        break;
    case Player::AttackDirection::Right:
        m_currentSlashAnimation = &m_animationSlashRight;
        break;
    }

    if (m_currentSlashAnimation) {
        m_currentSlashAnimation->setPosition(getLogicCenter());
        m_currentSlashAnimation->reset();
    }
}

void Player::updateAttackDirection(int x, int y) {
    float angle = std::atan2(y - p_position.y, x - p_position.x); // [-PI, PI]

    if (angle >= -M_PI / 4 && angle < M_PI / 4) {
        m_attackDirection = AttackDirection::Right;
    } else if (angle >= M_PI / 4 && angle < 3 * M_PI / 4) {
        m_attackDirection = AttackDirection::Down;
    } else if ((angle >= 3 * M_PI && angle <= M_PI) || (angle >= -M_PI  && angle < -3 * M_PI / 4)) {
        m_attackDirection = AttackDirection::Left;
    } else {
        m_attackDirection = AttackDirection::Up;
    }
}

void Player::updateAttackDirection() {
    if (m_isDownKeyDown && !m_isUpKeyDown) {
        m_attackDirection = AttackDirection::Down;
    } else if (m_isUpKeyDown && !m_isDownKeyDown) {
        m_attackDirection = AttackDirection::Up;
    } else if (p_isFacingLeft) {
        m_attackDirection = AttackDirection::Left;
    } else if (!p_isFacingLeft) {
        m_attackDirection = AttackDirection::Right;
    }
}
