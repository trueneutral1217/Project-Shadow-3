#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>

class Inventory {
public:
    void addItem(const std::string& item);
    void removeItem(const std::string& item);
    void showInventory() const;
    std::vector<std::string> items;
private:

};

#endif // INVENTORY_H
