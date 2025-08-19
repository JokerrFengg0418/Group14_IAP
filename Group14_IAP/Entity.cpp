#include "Entity.h"
#include <iostream>

Entity::Entity() : position(0, 0) {
    std::cout << "Entity Created." << std::endl;
}

Entity::Entity(int r, int c) : position(r, c) {
    std::cout << "Entity Created at (" << r << ", " << c << ")" << std::endl;
}

Entity::~Entity() {
    std::cout << "Entity Destroyed." << std::endl;
}

int Entity::getRow() const {
    return position.getRow();
}

void Entity::setRow(int r) {
    position.setPosition(r, position.getCol());
}

int Entity::getCol() const {
    return position.getCol();
}

void Entity::setCol(int c) {
    position.setPosition(position.getRow(), c);
}

void Entity::move() {
    // Example: Move diagonally down-right
    position.setPosition(position.getRow() + 1, position.getCol() + 1);
}

void Entity::EntityLocation() const {
    std::cout << "Entity Location: ("
        << position.getRow() << ", "
        << position.getCol() << ")" << std::endl;
}
