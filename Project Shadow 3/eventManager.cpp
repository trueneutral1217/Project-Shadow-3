#include "EventManager.h"

EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::registerEvent(const std::function<void()>&