#pragma once

#include <cstddef>
#include <iostream>
#include <sstream>
#include <vector>

class Life {
public:
  Life(const std::size_t width = 100, const std::size_t height = 100);
  Life random(const std::size_t width, const std::size_t height);
  ~Life();

  void set(const std::size_t idx);
  void clear(const std::size_t idx);

  void set(const std::size_t row, const std::size_t col);
  void clear(const std::size_t row, const std::size_t col);

  void set(const std::vector<std::size_t> idxs);
  void clear(const std::vector<std::size_t> idxs);

  bool get(const std::size_t idx) { return m_board[idx]; }
  void step();

  std::string toString() {
    int n{0};
    std::stringstream ss;
    for (bool c : m_board) {
      ss << (c ? '#' : ' ');
      if (n > 0 && n % m_width == 0)
        ss << std::endl;
      n++;
    }

    return ss.str();
  }

private:
  enum Change {
    Live,
    Dead,
  };

  struct BoardChange {
    Change change;
    std::size_t idx;
  };

  std::vector<bool> m_board;
  const std::size_t m_width;
  const std::size_t m_height;

  friend const void print(const Life &);
};
