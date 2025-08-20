#pragma once
#include "Position.h"

class Entity {
protected:

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
    int getHealth() const;
    int getDamage() const;

    int getRow() const;
    int getCol() const;
    Position getPosition() const;
    bool isAlive() const;

    // Setters
    void setHealth(int hp);
    void setDamage(int dmg);

    void setRow(int r);
    void setCol(int c);

    // functions
    virtual void move();
    virtual void attack();
    virtual void takeDamage();
    void EntityLocation() const;
};
