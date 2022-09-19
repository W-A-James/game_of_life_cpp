#pragma once

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class LifeTemplate {
public:
  LifeTemplate() {}
  LifeTemplate(const std::size_t w, const std::size_t h) {}
  LifeTemplate(const std::size_t w, const std::size_t h, const double rd) {}
  ~LifeTemplate(){};

  virtual void set(const std::vector<std::size_t> idxs) = 0;

  virtual void clear(const std::vector<std::size_t> idxs) = 0;

  virtual bool get(const std::size_t idx) = 0;

  virtual void step() = 0;
  virtual std::string toString() = 0;

};
