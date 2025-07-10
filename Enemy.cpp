#include "Enemy.h"
#include "Animation.h"
#include "Barb.h"
#include "Character.h"
#include "CharacterManager.h"
#include "AssetManager.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "EnemyStateNodes.h"
#include "Utils.h"

#include <algorithm>

Enemy::Enemy() {
    p_isFacingLeft = true; 
    p_position= { 1050, 200 };
    p_logicHeight = 150;

    p_hitBox->setSize({ 50, 80 });
    p_hurtBox->setSize({ 100, 180 });

    p_hitBox->setLayerSrc(CollisionLayer::None);
    p_hitBox->setLayerDst(CollisionLayer::Player);

    p_hurtBox->setLayerSrc(CollisionLayer::Enemy);
    p_hurtBox->setLayerDst(CollisionLayer::None);

    p_hurtBox->setOnCollide(
        [&]() {
            decreaseHp();
        } 
    );

    m_collisionBoxSilk = CollisionManager::instance()->createCollisionBox();
    m_collisionBoxSilk->setLayerSrc(CollisionLayer::None);
    m_collisionBoxSilk->setLayerDst(CollisionLayer::Player);
    m_collisionBoxSilk->setSize({ 225, 225 });
    m_collisionBoxSilk->setEnabled(false);

    {
        {
            AnimationGroup& animationAim = p_animationPool["aim"];

            Animation& animationAimLeft = animationAim.left;
            animationAimLeft.setInterval(0.05f);
            animationAimLeft.setLoop(false);
            animationAimLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationAimLeft.addFrame(AssetManager::instance()->findAtlas("enemy/aim"));

            Animation& animationAimRight = animationAim.right;
            animationAimRight.setInterval(0.05f);
            animationAimRight.setLoop(false);
            animationAimRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationAimRight.addFrame(AssetManager::instance()->findAtlas("enemy/aim"), true);
        }
        {
            AnimationGroup& animationDashInAir = p_animationPool["dash_in_air"];

            Animation& animationDashInAirLeft = animationDashInAir.left;
            animationDashInAirLeft.setInterval(0.05f);
            animationDashInAirLeft.setLoop(true);
            animationDashInAirLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDashInAirLeft.addFrame(AssetManager::instance()->findAtlas("enemy/dash_in_air"));

            Animation& animationDashInAirRight = animationDashInAir.right;
            animationDashInAirRight.setInterval(0.05f);
            animationDashInAirRight.setLoop(true);
            animationDashInAirRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDashInAirRight.addFrame(AssetManager::instance()->findAtlas("enemy/dash_in_air"), true);
        }
        {
            AnimationGroup& animationFall = p_animationPool["fall"];

            Animation& animationFallLeft = animationFall.left;
            animationFallLeft.setInterval(0.1f);
            animationFallLeft.setLoop(true);
            animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationFallLeft.addFrame(AssetManager::instance()->findAtlas("enemy/fall"));

            Animation& animationFallRight = animationFall.right;
            animationFallRight.setInterval(0.1f);
            animationFallRight.setLoop(true);
            animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationFallRight.addFrame(AssetManager::instance()->findAtlas("enemy/fall"), true);
        }
        {
            AnimationGroup& animationIdle = p_animationPool["idle"];

            Animation& animationIdleLeft = animationIdle.left;
            animationIdleLeft.setInterval(0.1f);
            animationIdleLeft.setLoop(true);
            animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationIdleLeft.addFrame(AssetManager::instance()->findAtlas("enemy/idle"));

            Animation& animationIdleRight = animationIdle.right;
            animationIdleRight.setInterval(0.1f);
            animationIdleRight.setLoop(true);
            animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationIdleRight.addFrame(AssetManager::instance()->findAtlas("enemy/idle"), true);
        }
        {
            AnimationGroup& animationJump = p_animationPool["jump"];

            Animation& animationJumpLeft = animationJump.left;
            animationJumpLeft.setInterval(0.1f);
            animationJumpLeft.setLoop(false);
            animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpLeft.addFrame(AssetManager::instance()->findAtlas("enemy/jump"));

            Animation& animationJumpRight = animationJump.right;
            animationJumpRight.setInterval(0.1f);
            animationJumpRight.setLoop(false);
            animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpRight.addFrame(AssetManager::instance()->findAtlas("enemy/jump"), true);
        }
        {
            AnimationGroup& animationRun = p_animationPool["run"];

            Animation& animationRunLeft = animationRun.left;
            animationRunLeft.setInterval(0.05f);
            animationRunLeft.setLoop(true);
            animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunLeft.addFrame(AssetManager::instance()->findAtlas("enemy/run"));

            Animation& animationRunRight = animationRun.right;
            animationRunRight.setInterval(0.05f);
            animationRunRight.setLoop(true);
            animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunRight.addFrame(AssetManager::instance()->findAtlas("enemy/run"), true);
        }
        {
            AnimationGroup& animationSquat = p_animationPool["squat"];

            Animation& animationSquatLeft = animationSquat.left;
            animationSquatLeft.setInterval(0.05f);
            animationSquatLeft.setLoop(false);
            animationSquatLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationSquatLeft.addFrame(AssetManager::instance()->findAtlas("enemy/squat"));

            Animation& animationSquatRight = animationSquat.right;
            animationSquatRight.setInterval(0.05f);
            animationSquatRight.setLoop(false);
            animationSquatRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationSquatRight.addFrame(AssetManager::instance()->findAtlas("enemy/squat"), true);
        }
        {
            AnimationGroup& animationThrowBarb = p_animationPool["throwBarb"];

            Animation& animationThrowBarbLeft = animationThrowBarb.left;
            animationThrowBarbLeft.setInterval(0.1f);
            animationThrowBarbLeft.setLoop(false);
            animationThrowBarbLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowBarbLeft.addFrame(AssetManager::instance()->findAtlas("enemy/throw_barb"));

            Animation& animationThrowBarbRight = animationThrowBarb.right;
            animationThrowBarbRight.setInterval(0.1f);
            animationThrowBarbRight.setLoop(false);
            animationThrowBarbRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowBarbRight.addFrame(AssetManager::instance()->findAtlas("enemy/throw_barb"), true);
        }
        {
            AnimationGroup& animationThrowSilk = p_animationPool["throwSilk"];

            Animation& animationThrowSilkLeft = animationThrowSilk.left;
            animationThrowSilkLeft.setInterval(0.1f);
            animationThrowSilkLeft.setLoop(true);
            animationThrowSilkLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSilkLeft.addFrame(AssetManager::instance()->findAtlas("enemy/throw_silk"));

            Animation& animationThrowSilkRight = animationThrowSilk.right;
            animationThrowSilkRight.setInterval(0.1f);
            animationThrowSilkRight.setLoop(true);
            animationThrowSilkRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSilkRight.addFrame(AssetManager::instance()->findAtlas("enemy/throw_silk"), true);
        }
        {
            AnimationGroup& animationThrowSword = p_animationPool["throwSword"];

            Animation& animationThrowSwordLeft = animationThrowSword.left;
            animationThrowSwordLeft.setInterval(0.05f);
            animationThrowSwordLeft.setLoop(false);
            animationThrowSwordLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSwordLeft.addFrame(AssetManager::instance()->findAtlas("enemy/throw_sword"));

            Animation& animationThrowSwordRight = animationThrowSword.right;
            animationThrowSwordRight.setInterval(0.05f);
            animationThrowSwordRight.setLoop(false);
            animationThrowSwordRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSwordRight.addFrame(AssetManager::instance()->findAtlas("enemy/throw_sword"), true);
        }
    }

    {
        m_animationSilk.setInterval(0.1f);
        m_animationSilk.setLoop(false);
        m_animationSilk.setAnchorMode(Animation::AnchorMode::Centered);
        m_animationSilk.addFrame(AssetManager::instance()->findAtlas("enemy/silk"));


        Animation& animationDashInAirLeft = m_animationDashInAirVfx.left;
        animationDashInAirLeft.setInterval(0.1f);
        animationDashInAirLeft.setLoop(false);
        animationDashInAirLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashInAirLeft.addFrame(AssetManager::instance()->findAtlas("enemy/vfx_dash_in_air"));

        Animation& animationDashInAirRight = m_animationDashInAirVfx.right;
        animationDashInAirRight.setInterval(0.1f);
        animationDashInAirRight.setLoop(false);
        animationDashInAirRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashInAirRight.addFrame(AssetManager::instance()->findAtlas("enemy/vfx_dash_in_air"), true);

        Animation& animationDashOnFloorLeft = m_animationDashOnFloorVfx.left;
        animationDashOnFloorLeft.setInterval(0.1f);
        animationDashOnFloorLeft.setLoop(false);
        animationDashOnFloorLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashOnFloorLeft.addFrame(AssetManager::instance()->findAtlas("enemy/vfx_dash_on_floor"));

        Animation& animationDashOnFloorRight = m_animationDashOnFloorVfx.right;
        animationDashOnFloorRight.setInterval(0.1f);
        animationDashOnFloorRight.setLoop(false);
        animationDashOnFloorRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashOnFloorRight.addFrame(AssetManager::instance()->findAtlas("enemy/vfx_dash_on_floor"), true);
    }

    {
        p_stateMachine.registerState("aim", new EnemyAimState());
        p_stateMachine.registerState("dashInAir", new EnemyDashInAirState());
        p_stateMachine.registerState("dashOnFloor", new EnemyDashOnFloorState());
        p_stateMachine.registerState("dead", new EnemyDeadState());
        p_stateMachine.registerState("fall", new EnemyFallState());
        p_stateMachine.registerState("idle", new EnemyIdleState());
        p_stateMachine.registerState("jump", new EnemyJumpState());
        p_stateMachine.registerState("run", new EnemyRunState());
        p_stateMachine.registerState("squat", new EnemySquatState());
        p_stateMachine.registerState("throwBarb", new EnemyThrowBarbState());
        p_stateMachine.registerState("thrwoSilk", new EnemyThrowSilkState());
        p_stateMachine.registerState("thrwoSword", new EnemyThrowSwordState());

        p_stateMachine.setEntry("idle");
    }
}

Enemy::~Enemy() {
    CollisionManager::instance()->destroyCollisionBox(m_collisionBoxSilk);
}

void Enemy::onUpdate(float deltaTime) {
    if (p_velocity.x >= 0.0001f) p_isFacingLeft = (p_velocity.x < 0);

    Character::onUpdate(deltaTime);

    p_hitBox->setPosition(getLogicCenter());

    if (m_isThrowingSilk) {
        m_collisionBoxSilk->setPosition(getLogicCenter());
        m_animationSilk.setPosition(getLogicCenter());
        m_animationSilk.update(deltaTime);
    }

    if (m_isDashingInAir || m_isDashingOnFloor) {
        m_currentDashAnimation->setPosition(m_isDashingInAir ? getLogicCenter() : p_position);
        m_currentDashAnimation->update(deltaTime);
    }

    for (Barb* barb : m_barbList) barb->update(deltaTime);
    for (Sword* sword : m_swordList) sword->update(deltaTime);

    m_barbList.erase(std::remove_if(
        m_barbList.begin(), m_barbList.end(),
        [](Barb* barb) {
            bool shouldRemove = !barb->checkIsValid(); 
            if (shouldRemove) delete barb;
            return shouldRemove;
        }),
        m_barbList.end()
    );
    m_swordList.erase(std::remove_if(
        m_swordList.begin(), m_swordList.end(),
        [](Sword* sword) {
            bool shouldRemove = !sword->checkIsValid(); 
            if (shouldRemove) delete sword;
            return shouldRemove;
        }),
        m_swordList.end()
    );
}

void Enemy::render(const Camera& camera) {
    for (Barb* barb : m_barbList) barb->render(camera);
    for (Sword* sword : m_swordList) sword->render(camera);

    Character::render(camera);

    if (m_isThrowingSilk) m_animationSilk.render(camera);
    if (m_isDashingInAir || m_isDashingOnFloor) m_currentDashAnimation->render(camera);
}

void Enemy::onHurt() {
    switch (randomInt(1, 3)) {
    case 1:
        playAudio("audio/enemy_hurt_1", false);
        break;
    case 2:
        playAudio("audio/enemy_hurt_2", false);
        break;
    case 3:
        playAudio("audio/enemy_hurt_3", false);
        break;
    }
}

void Enemy::throwBarbs() {
    int newBarbCount = randomInt(3, 6);
    if (m_barbList.size() >= 10) newBarbCount = 1;
    int widthGrid = 720 / newBarbCount;

    for (int i = 0; i < newBarbCount; i++) {
        Barb* newBarb = new Barb();
        int randomX = randomInt(widthGrid * i, widthGrid * (i + 1));
        int randomY = randomInt(250, 500);
        newBarb->setPosition({ (float)randomX, (float)randomY });
        m_barbList.push_back(newBarb);
    }
}

void Enemy::throwSword() {
    Sword* newSword = new Sword(getLogicCenter(), p_isFacingLeft);
    m_swordList.push_back(newSword);
}

void Enemy::onDash() {
    if (m_isDashingInAir) {
        m_currentDashAnimation = p_velocity.x < 0 ? &m_animationDashInAirVfx.left : &m_animationDashInAirVfx.right;
    } else {
        m_currentDashAnimation = p_velocity.x < 0 ? &m_animationDashOnFloorVfx.left : &m_animationDashOnFloorVfx.right;
    }

    m_currentDashAnimation->reset();
}

void Enemy::onThrowSilk() {
    m_animationSilk.reset();
}
