#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "Character.h"
#include <SDL2/SDL.h>

class CharacterManager {
    static CharacterManager* s_manager;

    Character* m_enemy = nullptr;
    Character* m_player = nullptr;

    CharacterManager();
    ~CharacterManager();

public:
    static CharacterManager* instance();

    Character* getEnemy() {
        return m_enemy;
    }

    Character* getPlayer() {
        return m_player;
    }

    void onInput(const SDL_Event& e);
    void update(float deltaTime);
    void render(const Camera& camera);
};

#endif
