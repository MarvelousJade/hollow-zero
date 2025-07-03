#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "StateNode.h"

#include <string>
#include <SDL2/SDL.h>
#include <unordered_map>

class StateMachine {
    bool m_shouldInit = true;
    StateNode* m_currentState = nullptr;
    std::unordered_map<std::string, StateNode*> m_statePool;

public:
    StateMachine();
    ~StateMachine();

    void onUpdate(float deltaTime);

    void setEntry(const std::string& id);
    void switchTo(const std::string& id);
    void registerState(const std::string& id, StateNode* StateNode);
};

#endif
