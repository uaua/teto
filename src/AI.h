#ifndef AI_H_
#define AI_H_

#include <vector>

class Field;
class MinoType;
class Decision;

class AI {
 public:
  AI() {}
  virtual void reset() = 0;
  virtual Decision think(const Field& f, const std::vector<MinoType>& s) = 0;
};

#endif  // AI_H_
