#ifndef QUESTSYSTEM_H
#define QUESTSYSTEM_H

#include <string>
#include <vector>

class Quest {
public:
    Quest(const std::string& title, const std::string& description, bool isCompleted = false);
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    bool isCompleted() const;
    void complete();
    
private:
    std::string title;
    std::string description;
    bool completed;
};

class QuestSystem {
public:
    void addQuest(const Quest& quest);
    void completeQuest(const std::string& title);
    void showQuests() const;

private:
    std::vector<Quest> quests;
};

#endif // QUESTSYSTEM_H
