#ifndef PRINTER_H_
#define PRINTER_H_

#include <cstdio>
#include <vector>

#include "Field.h"
#include "Mino.h"
#include "MinoType.h"

class Printer {
 private:
  const Field& f;
  MinoType hold;
  std::vector<MinoType> seq;

 public:
  Printer(const Field& f, const MinoType hold) : f(f), hold(hold), seq(0) {
  }
  Printer(const Field& f, const std::vector<MinoType>& seq, const MinoType hold)
    : f(f), hold(hold), seq(seq) {
  }
  void print() const;
};

#endif  // PRINTER_H_
