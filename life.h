#pragma once

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define ALIVE ('#')
#define DEAD (' ')

class Life {
public:
  Life(const std::size_t w, const std::size_t h);
  ~Life();

  Life(const std::size_t w, const std::size_t h, const double rd);

  void set(const std::vector<std::size_t> idxs) const;

  void clear(const std::vector<std::size_t> idxs) const;

  bool get(const std::size_t) const;

  void step() const;

  std::string toString() const;

  enum Change {
    Live,
    Dead,
  };

  struct BoardChange {
    Change change;
    std::size_t idx;
  };

  class LifeImpl;

private:
  LifeImpl *m_impl;
};
