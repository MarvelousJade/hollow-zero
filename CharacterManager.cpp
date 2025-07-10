#include "Enemy.h"
#include "Player.h"
#include "CharacterManager.h"
#include "BulletTimeManager.h"

CharacterManager* CharacterManager::s_manager = nullptr;

CharacterManager* CharacterManager::instance() {
    if (!s_manager) s_manager = new CharacterManager();

    return s_manager;
}

void CharacterManager::onInput(const SDL_Event& e) {
    m_player->onInput(e);
}

void CharacterManager::update(float deltaTime) {
    m_enemy->onUpdate(deltaTime);
    m_player->onUpdate(deltaTime);
}

void CharacterManager::render(const Camera& camera) {
    m_enemy->render(camera);
    BulletTimeManager::instance()->postProcess(camera);
    m_player->render(camera);
}

CharacterManager::CharacterManager() {
    m_enemy = new Enemy();
    m_player = new Player();
}

CharacterManager::~CharacterManager() {
    delete m_player;
    delete m_enemy;
}
