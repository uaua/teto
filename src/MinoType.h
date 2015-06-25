#ifndef MINOTYPE_H_
#define MINOTYPE_H_

class MinoType {
 public:
  int k;
  MinoType() {
    k = -1;
  }
  explicit MinoType(int k):k(k) {}
};

#endif  // MINOTYPE_H_
