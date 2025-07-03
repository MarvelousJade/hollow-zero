#include "StateMachine.h"

StateMachine::StateMachine() = default;

StateMachine::~StateMachine() = default;

void StateMachine::onUpdate(float deltaTime) {
    if (!m_currentState) return;

    if (m_shouldInit) {
        m_currentState->onEnter();
        m_shouldInit = false;
    }

    m_currentState->onUpdate(deltaTime);
}

void StateMachine::setEntry(const std::string& id) {
    m_currentState = m_statePool[id];
}

void StateMachine::switchTo(const std::string& id) {
    if (m_currentState) m_currentState->onExit();
    m_currentState = m_statePool[id];
    if (m_currentState) m_currentState->onEnter();
}

void StateMachine::registerState(const std::string& id, StateNode* stateNode) {
     m_statePool[id] = stateNode;
}
