#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include "Vector2.h"
#include "CollisionLayer.h"

#include <functional>

class CollisionManager;

class CollisionBox {
    friend class CollisionManager;

    Vector2 m_size;
    Vector2 m_position;
    bool m_isEnabled = true;
    std::function<void()> m_onCollide;
    CollisionLayer m_layerSrc = CollisionLayer::None;
    CollisionLayer m_layerDst = CollisionLayer::None;

    CollisionBox() = default;
    ~CollisionBox() = default;

public:
    void setEnabled(bool flag) {
        m_isEnabled = flag;
    }

    void setLayerSrc(CollisionLayer layer) {
        m_layerSrc = layer; 
    }

    void setLayerDst(CollisionLayer layer) {
        m_layerDst = layer; 
    }

    void setOnCollide(std::function<void()> onCollide) {
        m_onCollide = onCollide;
    }

    void setSize(const Vector2& size) {
        m_size = size;
    }

    const Vector2& getSize() const {
        return m_size;
    }

    void setPosition(const Vector2& position) {
        m_position = position;
    }
};

#endif
