#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <string>

class InventoryItem {
public:
    InventoryItem(const std::string& name, const std::string& description);
    const std::string& getName() const;
    const std::string& getDescription() const;

private:
    std::string name;
    std::string description;
};

#endif // INVENTORYITEM_H
