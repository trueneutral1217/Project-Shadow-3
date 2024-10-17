#include "Inventory.h"
#include <algorithm>
#include <iostream>

void Inventory::addItem(const std::string& item) {
    items.push_back(item);
}

void Inventory::removeItem(const std::string& item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        items.erase(it);
    }
}

void Inventory::showInventory() const {
    for (const auto& item : items) {
        std::cout << item << std::endl;
    }
}
