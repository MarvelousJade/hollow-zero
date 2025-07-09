#include "Barb.h"
#include "CollisionManager.h"
#include "AssetManager.h"
#include "CharacterManager.h"
#include "Utils.h"
#include "Vector2.h"
#include <cmath>

Barb::Barb() {
    m_diffPeriod = randomInt(0, 6);

    m_animationLoose.setInterval(0.15f);
    m_animationLoose.setLoop(true);
    m_animationLoose.setAnchorMode(Animation::AnchorMode::Centered);
    m_animationLoose.addFrame(AssetManager::instance()->findAtlas("enemy/barb_loose"));

    m_animationBreak.setInterval(0.1f);
    m_animationBreak.setLoop(false);
    m_animationBreak.setAnchorMode(Animation::AnchorMode::Centered);
    m_animationBreak.addFrame(AssetManager::instance()->findAtlas("enemy/barb_break"));
    m_animationBreak.setOnFinished([&]() { m_isValid = false; });

    m_collisionBox = CollisionManager::instance()->createCollisionBox();
    m_collisionBox->setLayerSrc(CollisionLayer::Enemy);
    m_collisionBox->setLayerDst(CollisionLayer::Player);
    m_collisionBox->setSize({ 20, 20 });
    m_collisionBox->setOnCollide([&]() { onBreak(); });

    m_timerIdle.setDuration(static_cast<float>(randomInt(3, 10)));
    m_timerIdle.setOneShot(true);
    m_timerIdle.setOnTimeOut(
        [&]() {
            if (stage == Stage::Idle) {
                stage = Stage::Aim;
                m_basePosition = m_currentPosition;
            }
        }
    );

    m_timerAim.setDuration(0.75);
    m_timerAim.setOneShot(true);
    m_timerAim.setOnTimeOut(
        [&]() {
            if (stage == Stage::Aim) {
                stage = Stage::Dash;
                const Vector2& playerPosition = CharacterManager::instance()->getPlayer()->getPosition();
                m_veclocity = (playerPosition - m_currentPosition).normalize() * SPEED_DASH;
            }
        }
    );
}

Barb::~Barb() {
    CollisionManager::instance()->destroyCollisionBox(m_collisionBox);
}

void Barb::update(float deltaTime) {
    if (stage == Stage::Idle) m_timerIdle.update(deltaTime);
    if (stage == Stage::Aim) m_timerAim.update(deltaTime);

    // Update movement
    m_totalDeltaTime += deltaTime; 
    switch (stage) {
    case Barb::Stage::Idle:
        m_currentPosition.y = m_basePosition.y + sin(m_totalDeltaTime * 2 + m_diffPeriod) * 30;
        break;
    case Barb::Stage::Aim:
        m_currentPosition.x = m_basePosition.x + randomInt(-10, 10);
        break;
    case Barb::Stage::Dash:
        m_currentPosition += m_veclocity * deltaTime; 
        if ( m_currentPosition.y >= CharacterManager::instance()->getPlayer()->getFloorY()){
            onBreak();
        };

        if ( m_currentPosition.y <= 0 ){
            m_isValid = false;
        };
        break;
    }
    m_collisionBox->setPosition(m_currentPosition);

    // Update animation
    m_currentAnimation = (stage == Stage::Break ? &m_animationBreak : &m_animationLoose);
    m_currentAnimation->setPosition(m_currentPosition);
    m_currentAnimation->update(deltaTime);
}

void Barb::render(const Camera& camera) {
    m_currentAnimation->render(camera);
}

void Barb::onBreak() {
    if (stage == Stage::Break) return;

    stage = Stage::Break; 
    m_collisionBox->setEnabled(false);
    playAudio("audio/barb_break", false);
}
