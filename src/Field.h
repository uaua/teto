#ifndef FIELD_H_
#define FIELD_H_

#include <cstring>

#include "Cell.h"

static const int FIELD_HEIGHT = 21;
static const int FIELD_WIDTH = 10;

class Cell;

class Field {
 public:
  Field() {
    clear();
  }
  Field(const Field& f) {
    memcpy(field_, f.field_, sizeof(f.field_));
  }
  void clear() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      for (int j = 0; j < FIELD_WIDTH; j++) field_[i][j].setCell(0);
    }
  }
  Cell get(int x, int y) const { return field_[y][x]; }
  void set(int x, int y, int v) {
    field_[y][x].setCell(v);
  }
  int top(int x) const {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      if (field_[i][x].cell()) return i;
    }
    return FIELD_HEIGHT;
  }
  int height(int x) const {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      if (field_[i][x].cell()) return FIELD_HEIGHT-i;
    }
    return 0;
  }

 private:
  Cell field_[FIELD_HEIGHT][FIELD_WIDTH];
};

#endif  // FIELD_H_
