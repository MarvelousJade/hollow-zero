#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "CollisionBox.h"
#include "Camera.h"

#include <vector>

class CollisionManager {
    static CollisionManager* s_collisionManager;
    std::vector<CollisionBox*> m_collisionBoxList;

    CollisionManager();
    ~CollisionManager();

public:
    static CollisionManager* instance();

    CollisionBox* createCollisionBox();
    void destroyCollisionBox(CollisionBox* collisionBox);

    void handleCollision();
    void debugRender(const Camera& camera);
};

#endif
