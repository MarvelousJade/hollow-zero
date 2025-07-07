#ifndef STATE_NODE_H
#define STATE_NODE_H

class StateNode {
public:
    StateNode() = default;
    ~StateNode() = default;
    
    virtual void onEnter() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onExit() = 0;
};

#endif
