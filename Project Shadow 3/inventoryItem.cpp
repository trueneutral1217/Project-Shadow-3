#include "InventoryItem.h"

InventoryItem::InventoryItem(const std::string& name, const std::string& description)
    : name(name), description(description) {}

const std::string& InventoryItem::getName() const {
    return name;
}

const std::string& InventoryItem::getDescription() const {
    return description;
}
