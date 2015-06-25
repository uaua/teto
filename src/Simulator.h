#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <algorithm>
#include <functional>
#include <vector>

class Field;
class Decision;
class MinoType;

class Simulator {
 public:
  typedef std::function<void(const Field&,
                             const std::vector<Decision>&,
                             int,
                             int,
                             const MinoType&)> Callback;

  static void iterate(const Field& f,
                      const std::vector<MinoType>& s,
                      const Callback& callback,
                      const MinoType& hold,
                      bool prohibits[],
                      int len = 0);
  static int putMino(Field* f, const MinoType& ss, const Decision& d);

 private:
  static void iterateInternal(const Field& f,
                              const std::vector<MinoType>& s,
                              const Callback& callback,
                              int depth,
                              std::vector<Decision> d,
                              const MinoType& hold,
                              bool prohibits[],
                              const int line,
                              const int len,
                              bool preHold = false);
};

#endif  // SIMULATOR_H_
