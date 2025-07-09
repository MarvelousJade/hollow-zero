#ifndef BULLET_TIME_MANAGER_H
#define BULLET_TIME_MANAGER_H

#include "Camera.h"

class BulletTimeManager {
public:
    enum class Status {
        Entering,
        Exiting
    };

    static BulletTimeManager* instance();

    void postProcess(const Camera& camera);
    void setStatus(Status status);

    float update(float deltaTime);

    const Status& getStatus() const { return m_status; };

private:
    static BulletTimeManager* s_manager;

    float m_progress = 0;
    Status m_status = Status::Exiting;
    const float SPEED_PROGRESS = 2.0f;
    const float DESTINATION_DELTA_FACTOR = 0.35f;
    const float DESTINATION_COLOR_FACTOR = 0.35f;

    BulletTimeManager();
    ~BulletTimeManager();

    float lerp(float start, float end, float t) {
        return (1 - t) * start + t * end;
    }
};

#endif
