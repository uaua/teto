#include "Printer.h"

void Printer::print() const {
  const int H = FIELD_HEIGHT+10;
  const int W = FIELD_WIDTH+30;
  char s[H][W];
#ifdef DEBUG
  puts("print");
#endif
  memset(s, ' ', sizeof(s));
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    // printf("#");
    s[y][0] = '#';
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (f.get(x, y).cell()) {
        // printf("o");
        s[y][x+1] = 'o';
      } else {
        // printf(" ");
        s[y][x+1] = ' ';
      }
    }
    s[y][FIELD_WIDTH+1] = '#';
    // puts("#");
  }
  if (hold.k >= 0) {
    const auto& a = mino[hold.k].rotate(0);
    for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < a[i].size(); j++) {
        s[1+i][FIELD_WIDTH+3+9+j] = a[i][j] ? 'o' : ' ';
      }
    }
  }
  for (size_t ii = 0; ii < seq.size(); ii++) {
    const auto& a = mino[seq[ii].k].rotate(0);
    for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < a[i].size(); j++) {
        s[1+ii*3+i][FIELD_WIDTH+3+j] = a[i][j] ? 'o' : ' ';
      }
    }
  }
  for (int i = 0; i < FIELD_WIDTH+2; i++) {
    // puts("############");
    s[FIELD_HEIGHT][i] = '#';
  }
  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      putchar(s[y][x]);
    }
    puts("");
  }
#ifdef DEBUG
  puts("endprint");
#endif
}
