#pragma once
#include "life.h"
#include <array>
#include <random>

class Life {
public:
  Life(const std::size_t w, const std::size_t h);
  ~Life();

  Life(const std::size_t w, const std::size_t h, const double rd);

  void set(const std::vector<std::size_t> idxs);

  void clear(const std::vector<std::size_t> idxs);

  void step();

  bool get(const std::size_t);

  std::string toString();

  enum Change {
    Live,
    Dead,
  };

  struct BoardChange {
    Change change;
    std::size_t idx;
  };

private:
  std::vector<bool> m_board;
  const std::size_t m_width;
  const std::size_t m_height;
};
