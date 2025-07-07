#ifndef STATE_NODE_H
#define STATE_NODE_H

class StateNode {
public:
    StateNode() = default;
    ~StateNode() = default;
    
    virtual void onEnter();
    virtual void onUpdate(float deltaTime);
    virtual void onExit();
};

#endif
