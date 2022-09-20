#include "neighbour_refs_par.h"
#include <future>
#include <random>
#include <shared_mutex>
#include <thread>

using namespace std;

Life::Life(const size_t w, const size_t h) : m_width(w), m_height(h) {
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

    m_neighbours[iTimes8] = topRow * m_width + leftCol;         // Top left
    m_neighbours[iTimes8 + 1] = topRow * m_width;               // Above
    m_neighbours[iTimes8 + 2] = topRow * m_width + rightCol;    // Top right
    m_neighbours[iTimes8 + 3] = rowNum * m_width + leftCol;     // Left
    m_neighbours[iTimes8 + 4] = rowNum * m_width + rightCol;    // Right
    m_neighbours[iTimes8 + 5] = bottomRow * m_width + leftCol;  // Bottom Left
    m_neighbours[iTimes8 + 6] = bottomRow * m_width;            // Below
    m_neighbours[iTimes8 + 7] = bottomRow * m_width + rightCol; // Bottom Right

    m_board[idx] = false;
  }
  m_processor_count = std::thread::hardware_concurrency();
  m_run_par = (m_width * m_height >= 1024 * 1024) && (m_processor_count) > 1;
  m_chunk_size = (m_width * m_height) / (m_processor_count - 1);
  m_rem = m_width * m_height % (m_processor_count - 1);
}

Life::Life(const size_t w, const size_t h, const double rd) : Life(w, h) {
  std::random_device r;
  std::default_random_engine e(r());

  std::uniform_real_distribution uniformDist(0.0, 1.0);
  for (int idx{0}; idx < w * h; idx++) {
    m_board[idx] = uniformDist(e) < rd;
  }
}

Life::~Life() {
  delete[] m_neighbours;
  delete[] m_board;
}

void Life::step() {
  vector<BoardChange> changes{};

  auto _step = [this](size_t start, size_t end) -> vector<BoardChange> * {
    vector<BoardChange> *changes = new vector<BoardChange>;
    for (size_t idx{start}; idx < end; idx++) {
      int numNeighbours{0};
      int neighbourIdxStart = 8 * idx;

      for (size_t offset{0}; offset < 8; offset++) {
        numNeighbours +=
            static_cast<int>(m_board[m_neighbours[neighbourIdxStart + offset]]);
      }

      bool live = m_board[idx];
      if (live && ((numNeighbours < 2) || (numNeighbours > 3))) {
        changes->push_back(BoardChange{Change::Dead, idx});
      } else if (!live && (numNeighbours == 3)) {
        changes->push_back(BoardChange{Change::Live, idx});
      }
    }
    return changes;
  };

  if (m_run_par) {
    int tNum{0};
    vector<future<vector<BoardChange> *>> handles{};

    for (; tNum < m_processor_count - 1; tNum++) {
      handles.push_back(std::async(std::launch::async, _step,
                                   tNum * m_chunk_size,
                                   tNum * m_chunk_size + m_chunk_size - 1));
    }
    if (m_rem > 0) {
      handles.push_back(std::async(std::launch::async, _step,
                                   tNum * m_chunk_size,
                                   tNum * m_chunk_size + m_rem));
    }

    for (auto &handle : handles) {
      for (auto change : *handle.get()) {
        m_board[change.idx] = (change.change == Change::Live);
      }
    }

  } else {
    for (size_t idx{0}; idx < m_width * m_height; idx++) {
      int numNeighbours{0};
      int neighbourIdxStart = 8 * idx;

      for (size_t offset{0}; offset < 8; offset++) {
        numNeighbours +=
            static_cast<int>(m_board[m_neighbours[neighbourIdxStart + offset]]);
      }

      bool live = m_board[idx];
      if (live && ((numNeighbours < 2) || (numNeighbours > 3))) {
        changes.push_back(BoardChange{Change::Dead, idx});
      } else if (!live && (numNeighbours == 3)) {
        changes.push_back(BoardChange{Change::Live, idx});
      }
    }
    for (auto change : changes) {
      m_board[change.idx] = (change.change == Change::Live);
    }
  }
}

void _step(bool *cells, size_t start, size_t end, std::mutex *l) {
  for (size_t idx{start}; idx < end; idx++) {
  }
}

void Life::set(const vector<size_t> idxs) {
  for (auto idx : idxs) {
    m_board[idx] = true;
  }
}

void Life::clear(const vector<size_t> idxs) {
  for (auto idx : idxs) {
    m_board[idx] = false;
  }
}

bool Life::get(const size_t idx) { return m_board[idx]; }
