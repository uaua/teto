#ifndef DECISION_H_
#define DECISION_H_

class Decision {
 public:
  int x, r, y, h;
  Decision() {}
  Decision(int x, int r, int y, int h) : x(x), r(r), y(y), h(h) {
  }
};

#endif  // DECISION_H_
