#include "Player.h"
#include "CharacterManager.h"

CharacterManager* CharacterManager::s_manager = nullptr;

CharacterManager* CharacterManager::instance() {
    if (!s_manager) s_manager = new CharacterManager();

    return s_manager;
}

void CharacterManager::onInput(const SDL_Event& e) {
    m_player->onInput(e);
}

void CharacterManager::update(float deltaTime) {
    m_player->onUpdate(deltaTime);
}

void CharacterManager::render(const Camera& camera) {
    m_player->render(camera);
}

CharacterManager::CharacterManager() {
    m_player = new Player();
}

CharacterManager::~CharacterManager() {
    delete m_player;
    delete m_enemy;
}
