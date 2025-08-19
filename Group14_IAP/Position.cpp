#include "Position.h"
Position::Position(int r, int c) : row(r), col(c) {}
int Position::getRow() const { return row; }
int Position::getCol() const { return col; }
void Position::setPosition(int r, int c) { row = r; col = c; }
