#ifndef CELL_H_
#define CELL_H_

class Cell {
 public:
  Cell() { cell_ = 0; }
  int cell() const { return cell_; }
  void setCell(int cell) { cell_ = cell; }
 private:
  int cell_;
};

#endif  // CELL_H_
