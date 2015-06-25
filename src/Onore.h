#ifndef ONORE_H_
#define ONORE_H_

#include <stdint.h>
#include <vector>

#include <Windows.h>

#include "AI.h"
#include "MinoType.h"

class Onore : public AI {
 private:
  const int hlim = 13;
  MinoType hold_;
  int limits;
  int plen;
  int evalHeight(const Field& f) const;
  int evalHole(const Field& f) const;

 public:
  Onore() {
    reset();
  }
  void reset() override;
  int64_t eval(const Field& f, int line, int len) const;
  Decision think(const Field& f, const std::vector<MinoType>& s) override;
  MinoType hold() const { return hold_; }
};

#endif  // ONORE_H_
