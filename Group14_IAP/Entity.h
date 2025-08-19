#pragma once
#include "Position.h"

class Entity {
private:

    Position position; // Stores row & col of Entity

    int Damage;

    int Range;

    int Health;



public:

    // default constructor
    Entity();

    // overloaded constructor
    Entity(int r, int c);

    // default destructor
    ~Entity();

    // Getter & Setters
    int getRow() const;
    void setRow(int r);
    int getCol() const;
    void setCol(int c);

    // functions
    void move();
    void EntityLocation() const;
};
