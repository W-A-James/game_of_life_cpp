#include "life.h"
#include <random>

class Life::LifeImpl {
public:
  LifeImpl(const std::size_t w, const std::size_t h) : m_height(h), m_width(w) {
    m_board = new bool[w * h];
    m_neighbours = new size_t[w * h * 8];
    for (size_t idx{0}; idx < w * h; idx++) {
      int neighbourIdx{0};
      size_t rowNum = idx / m_width;
      size_t colNum = idx % m_width;

      size_t leftCol = colNum - 1 > colNum ? m_width - 1 : colNum - 1;
      size_t rightCol = colNum + 1 == m_width ? 0 : colNum + 1;
      size_t topRow = rowNum - 1 > rowNum ? m_height - 1 : rowNum - 1;
      size_t bottomRow = rowNum + 1 == m_height ? 0 : rowNum + 1;

      size_t iTimes8 = 8 * idx;

      m_neighbours[iTimes8] = topRow * m_width + leftCol;        // Top left
      m_neighbours[iTimes8 + 1] = topRow * m_width;              // Above
      m_neighbours[iTimes8 + 2] = topRow * m_width + rightCol;   // Top right
      m_neighbours[iTimes8 + 3] = rowNum * m_width + leftCol;    // Left
      m_neighbours[iTimes8 + 4] = rowNum * m_width + rightCol;   // Right
      m_neighbours[iTimes8 + 5] = bottomRow * m_width + leftCol; // Bottom Left
      m_neighbours[iTimes8 + 6] = bottomRow * m_width;           // Below
      m_neighbours[iTimes8 + 7] =
          bottomRow * m_width + rightCol; // Bottom Right

      m_board[idx] = false;
    }
  }

  LifeImpl(const std::size_t w, const std::size_t h, const double rd)
      : LifeImpl(w, h) {
    std::random_device r;
    std::default_random_engine e(r());

    std::uniform_real_distribution uniformDist(0.0, 1.0);
    for (int idx{0}; idx < w * h; idx++) {
      m_board[idx] = uniformDist(e) < rd;
    }
  }

  void step() {
    std::vector<Life::BoardChange> changes{};
    for (size_t idx{0}; idx < m_width * m_height; idx++) {
      int numNeighbours{0};
      int neighbourIdxStart = 8 * idx;

      for (size_t offset{0}; offset < 8; offset++) {
        numNeighbours +=
            static_cast<int>(m_board[m_neighbours[neighbourIdxStart + offset]]);
      }

      if (m_board[idx] && ((numNeighbours < 2) || (numNeighbours > 3))) {
        changes.push_back(Life::BoardChange{Life::Change::Dead, idx});
      } else if (!m_board[idx] && (numNeighbours == 3)) {
        changes.push_back(Life::BoardChange{Life::Change::Live, idx});
      }
    }

    for (auto change : changes) {
      m_board[change.idx] = (change.change == Life::Change::Live);
    }
  }

  std::string toString() {
    std::stringstream ss;
    for (int n{0}; n < m_width * m_height; n++) {
      bool c = m_board[n];
      ss << (c ? ALIVE : DEAD);
      if (n > 0 && n % m_width == 0)
        ss << std::endl;
    }

    return ss.str();
  }

  void set(const std::vector<size_t> idxs) {
    for (auto idx : idxs) {
      m_board[idx] = true;
    }
  }

  void clear(const std::vector<size_t> idxs) {
    for (auto idx : idxs) {
      m_board[idx] = false;
    }
  }

  bool get(const size_t idx) { return m_board[idx]; }

  ~LifeImpl() {
    delete[] m_neighbours;
    delete[] m_board;
  }

private:
  bool *m_board;
  size_t *m_neighbours;
  const size_t m_width;
  const size_t m_height;
};

Life::Life(const size_t w, const size_t h) { m_impl = new LifeImpl(w, h); }

Life::Life(const size_t w, const size_t h, const double rd) {
  m_impl = new LifeImpl(w, h, rd);
}

Life::~Life() { delete m_impl; }

void Life::step() const { m_impl->step(); }

void Life::set(const std::vector<size_t> idxs) const { m_impl->set(idxs); }

void Life::clear(const std::vector<size_t> idxs) const { m_impl->clear(idxs); }

bool Life::get(const size_t idx) const { return m_impl->get(idx); }

std::string Life::toString() const { return m_impl->toString(); }
