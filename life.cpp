#include "life.h"
#include <array>
#include <random>
class Life;

class Life : public LifeTemplate {
public:
  Life(const std::size_t w, const std::size_t h) : m_height(h), m_width(w) {
    std::size_t size{w * h};
    m_board.reserve(w * h);
    for (std::size_t i{0}; i < w * h; i++) {
      m_board.push_back(false);
    }
  }

  virtual ~Life() { m_board.clear(); }

  Life random(const std::size_t w = 100, const std::size_t h = 100) {
    std::random_device r;
    std::default_random_engine e(r());

    Life life{w, h};
    life.m_board.reserve(w * h);
    std::uniform_real_distribution uniformDist(0.0, 1.0);
    for (int idx{0}; idx < w * h; idx++) {
      life.m_board.push_back(uniformDist(e) > 0.5);
    }

    return life;
  }

  virtual void set(const std::size_t idx) { m_board[idx] = true; }

  virtual void clear(const std::size_t idx) { m_board[idx] = false; }

  virtual void set(const std::vector<std::size_t> idxs) {
    for (auto idx : idxs) {
      m_board[idx] = true;
    }
  }

  virtual void clear(const std::vector<std::size_t> idxs) {
    for (auto idx : idxs) {
      m_board[idx] = false;
    }
  }

  virtual void step() {
    std::vector<BoardChange> changes;
    // populate changes vector
    for (std::size_t idx{0}; idx < m_board.size(); idx++) {
      int numLiveNeighbours{0};

      std::size_t rowNum = idx / m_width;
      std::size_t colNum = idx % m_width;

      std::array<std::size_t, 8> neighbourIdxs{};
      std::size_t leftCol = colNum - 1 > colNum ? m_width - 1 : colNum - 1;
      std::size_t rightCol = colNum + 1 == m_width ? 0 : colNum + 1;
      std::size_t topRow = rowNum - 1 > rowNum ? m_height - 1 : rowNum - 1;
      std::size_t bottomRow = rowNum + 1 == m_height ? 0 : rowNum + 1;

      // TODO: fix with wrapping
      // Top left
      neighbourIdxs[0] = topRow * m_width + leftCol;
      // Above
      neighbourIdxs[1] = topRow * m_width;
      // Top right
      neighbourIdxs[2] = topRow * m_width + rightCol;
      // Left
      neighbourIdxs[3] = rowNum * m_width + leftCol;
      // Right
      neighbourIdxs[4] = rowNum * m_width + rightCol;
      // Bottom Left
      neighbourIdxs[5] = bottomRow * m_width + leftCol;
      // Below
      neighbourIdxs[6] = bottomRow * m_width;
      // Bottom Right
      neighbourIdxs[7] = bottomRow * m_width + rightCol;

      for (auto idx : neighbourIdxs) {
        numLiveNeighbours += static_cast<int>(m_board[idx]);
      }

      if (m_board[idx] && (numLiveNeighbours < 2 || numLiveNeighbours > 3)) {
        changes.push_back(BoardChange{Change::Dead, idx});
      } else if (!m_board[idx] && (numLiveNeighbours == 3)) {
        changes.push_back(BoardChange{Change::Live, idx});
      }
    }
    // iterate over changes vector applying changes to board
    for (auto change : changes) {
      m_board[change.idx] = change.change == Change::Live;
    }
  }

  virtual std::string toString() {
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
};
