#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

class EventManager {
public:
    static EventManager& getInstance();
    void registerEvent(const std::function<void()>& event);
    void triggerEvents();

private:
    EventManager() = default;
    std::vector<std::function<void()>> events;
};

#endif // EVENTMANAGER_H
