#pragma once
#include <array>
#include <random>

class Life {
public:
  Life(const std::size_t w, const std::size_t h);
  ~Life();

  Life(const std::size_t w, const std::size_t h, const double rd);

  void set(const std::vector<std::size_t> idxs);

  void clear(const std::vector<std::size_t> idxs);

  bool get(const std::size_t);

  void step();

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
  bool *m_board;
  size_t *m_neighbours;
  const size_t m_width;
  const size_t m_height;
};
