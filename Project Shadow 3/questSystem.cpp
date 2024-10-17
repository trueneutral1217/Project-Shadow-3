#include "QuestSystem.h"
#include <iostream>

Quest::Quest(const std::string& title, const std::string& description, bool isCompleted)
    : title(title), description(description), completed(isCompleted) {}

const std::string& Quest::getTitle() const {
    return title;
}

const std::string& Quest::getDescription() const {
    return description;
}

bool Quest::isCompleted() const {
    return completed;
}

void Quest::complete() {
    completed = true;
}

void QuestSystem::addQuest(const Quest& quest) {
    quests.push_back(quest);
}

void QuestSystem::completeQuest(const std::string& title) {
    for (auto& quest : quests) {
        if (quest.getTitle() == title) {
            quest.complete();
            break;
        }
    }
}

void QuestSystem::showQuests() const {
    for (const auto& quest : quests) {
        std::cout << "Title: " << quest.getTitle() << "\nDescription: " << quest.getDescription() << "\nCompleted: " << (quest.isCompleted() ? "Yes" : "No") << "\n\n";
    }
}
