#ifndef TIMER_H
#define TIMER_H

#include <functional>

class Timer {
    float m_elapsedTime = 0;
    float m_duration = 0;
    bool m_isPaused = false;
    bool m_hasTriggered = false;
    bool m_isOneShot =false;
    std::function<void()> m_onTimeOut;
public:
    Timer() = default;
    ~Timer() = default;

    void restart() {
        m_elapsedTime = 0;
        m_hasTriggered = false;
    }

    void setDuration(float time) {
        m_duration = time;
    }

    void setOneShot(bool flag) {
        m_isOneShot = flag;
    }

    void setOnTimeOut(std::function<void()> onTimeOut) {
        this->m_onTimeOut = onTimeOut;
    }

    void pause() {
        m_isPaused = true;
    }

    void resume() {
        m_isPaused = false;
    }

    void update(float deltaTime) {
        if (m_isPaused) return;

        m_elapsedTime += deltaTime;
        if (m_elapsedTime >= m_duration) {
            bool canShot = (!m_isOneShot || (m_isOneShot && !m_hasTriggered));
            m_hasTriggered = true;
            if(canShot && m_onTimeOut) m_onTimeOut();
            m_elapsedTime -= m_duration;
        }
    }
};

#endif
