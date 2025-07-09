#include "Sword.h"
#include "AssetManager.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

Sword::Sword(const Vector2& position, bool shouldMoveLeft) {
    m_animation.setInterval(0.1f);
    m_animation.setLoop(true);
    m_animation.setAnchorMode(Animation::AnchorMode::Centered);
    m_animation.addFrame(AssetManager::instance()->findAtlas("enemy/sword"), !shouldMoveLeft);

    m_collisionBox = CollisionManager::instance()->createCollisionBox();
    m_collisionBox->setLayerSrc(CollisionLayer::None);
    m_collisionBox->setLayerDst(CollisionLayer::Player);
    m_collisionBox->setSize({ 195, 10 });

    m_position = position;
    m_veclocity = { shouldMoveLeft ? -SPEED_MOVE : SPEED_MOVE, 0 };
}

Sword::~Sword() {
    CollisionManager::instance()->destroyCollisionBox(m_collisionBox);
}

void Sword::update(float deltaTime) {
    m_position += m_veclocity * deltaTime;
    m_animation.setPosition(m_position);
    m_collisionBox->setPosition(m_position);

    m_animation.update(deltaTime);

    if(m_position.x <= -200 || m_position.x >= 720 + 200) m_isValid = false;
}

void Sword::render(const Camera &camera) {
    m_animation.render(camera);
}
