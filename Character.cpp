#include "Character.h"
#include "CollisionManager.h"

Character::Character() {
    p_hitBox = CollisionManager::instance()->createCollisionBox();
    p_hurtBox = CollisionManager::instance()->createCollisionBox();

    p_timerInvulnerableStatus.setDuration(1.0f);
    p_timerInvulnerableStatus.setOneShot(true);
    p_timerInvulnerableStatus.setOnTimeOut(
        [&]() {
            p_isInvulnerable = false;
        }
    );

    p_timerInvulnerableBlink.setDuration(0.075f);
    p_timerInvulnerableBlink.setOneShot(false);
    p_timerInvulnerableBlink.setOnTimeOut(
        [&]() {
            p_isBlinkInvisible = !p_isBlinkInvisible;
        }
    );
}

Character::~Character() {
    CollisionManager::instance()->destroyCollisionBox(p_hitBox);
    CollisionManager::instance()->destroyCollisionBox(p_hurtBox);
}

void Character::decreaseHp() {
    if (p_isInvulnerable) return;

    p_hp -= 1;
    if (p_hp > 0) makeInvulnerable();
    onHurt();
}

void Character::onInput(const SDL_Event& event) {}

void Character::onUpdate(float deltaTime) {
    p_stateMachine.onUpdate(deltaTime);

    if (p_hp <=0) p_velocity.x = 0;
    if (p_shouldEnableGravity) p_velocity.y += GRAVITY_Y * deltaTime;

    p_position += p_velocity * deltaTime;

    if (isOnFloor()) {
        p_position.y = FLOOR_Y;
        p_velocity.y = 0; 
    }

    if (p_position.x <= 0) p_position.x = 0;
    if (p_position.x >= 1280) p_position.x = 1280.0f;

    p_hurtBox->setPosition(getLogicCenter());

    p_timerInvulnerableStatus.update(deltaTime);

    if (p_isInvulnerable) p_timerInvulnerableBlink.update(deltaTime);

    if (!p_currentAnimation) return;

    Animation& animation = (
        p_isFacingLeft ? p_currentAnimation->left : p_currentAnimation->right
    );
    animation.update(deltaTime);
    animation.setPosition(p_position);
}

void Character::render(const Camera& camera) {
    if (!p_currentAnimation || (p_isInvulnerable && p_isBlinkInvisible)) return;

    (p_isFacingLeft ? p_currentAnimation->left : p_currentAnimation->right).render(camera);
}


void Character::onHurt() {}

void Character::switchState(const std::string& id) {
    p_stateMachine.switchTo(id);
}

void Character::setAnimation(const std::string& id) {
    p_currentAnimation = &p_animationPool[id];

    p_currentAnimation->left.reset();
    p_currentAnimation->right.reset();
}
