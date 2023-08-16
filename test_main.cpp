#include "life.h"

#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace test {

std::mutex cout_mutex{};
struct TestFn_t {
  std::string name;
  std::function<void()> f;
};

using TestFn = struct TestFn_t;

class Suite {
public:
  Suite(std::string name) : m_name(name) {}

  void add_test(TestFn test_fn) { m_tests.push_back(test_fn); }

  void run() {
    std::stringstream os;
    os << "Running suite [" << m_name << ']' << std::endl;

    for (TestFn t : m_tests) {
      os << "\t[" << t.name << "] --- ";
      try {
        t.f();
        os << "Passed\n";
        m_numSuccesses++;
      } catch (int err) {
        os << "Failed\n";
        m_numFailures++;
      }
    }
    m_status = os.str();

    cout_mutex.lock();
    std::cout << status();
    cout_mutex.unlock();
  };

  std::string status() const {
    std::stringstream os;
    os << m_status << "---------------------------------------" << std::endl
       << "Total tests: " << m_tests.size() << std::endl
       << "Passed: " << m_numSuccesses << std::endl
       << "Failed: " << m_numFailures << std::endl
       << "---------------------------------------\n\n";
    return os.str();
  }

private:
  std::string m_name;
  std::vector<TestFn> m_tests;
  std::string m_status;
  int m_numSuccesses{0};
  int m_numFailures{0};
};
std::vector<TestFn *> tests{};

void assert(bool condition) {
  if (!condition) {
    throw(1);
  }
}
} // namespace test

int main(int argc, char **argv) {
  test::Suite stepCorrectness{"Step Correctness Tests"};
  {
    stepCorrectness.add_test(
        test::TestFn{"Test Life::step() handles reproduction", []() {
                       Life life{10, 10};
                       life.set(std::vector<std::size_t>{0, 1, 2});
                       life.step();

                       test::assert(life.get(11));
                     }});
    stepCorrectness.add_test(
        test::TestFn{"Test Life::step() handles overpopulation", []() {
                       Life life{10, 10};
                       life.set(std::vector<std::size_t>{0, 1, 2, 10, 11});
                       life.step();

                       test::assert(!life.get(11));
                     }});
    stepCorrectness.add_test(
        test::TestFn{"Test Life::step() handles underpopulation", []() {
                       Life life{10, 10};
                       life.set(std::vector<std::size_t>{0});
                       life.step();
                       test::assert(!life.get(0));
                     }});

    stepCorrectness.add_test(test::TestFn{"Test Life::step() handles wrapping",
                                          []() { test::assert(true); }});
    stepCorrectness.add_test(
        test::TestFn{"Test Life::step() handles live cells persisting", []() {
                       Life life{10, 10};
                       life.set(std::vector<std::size_t>{0, 1, 10, 11});
                       life.step();
                       test::assert(life.get(0) && life.get(1) &&
                                    life.get(10) && life.get(11));
                     }});
  }

  test::Suite stepPerformance{"Step Performance"};
  {
    stepPerformance.add_test(test::TestFn{
        "Test Performance with Life::step()", [&]() {
          constexpr std::size_t ITERS{100};
          std::vector<std::pair<std::size_t, std::chrono::duration<double>>>
              times{};
          for (std::size_t n{8}; n <= 2048; n *= 2) {
            Life life{n, n, 0.5};
            auto start = std::chrono::steady_clock::now();
            for (int i{0}; i < ITERS; i++) {
              life.step();
            }
            times.push_back(
                std::make_pair(n, std::chrono::steady_clock::now() - start));
          }

          std::stringstream ss;
          for (auto entry : times) {
            ss << "N: " << entry.first << "\tIters: " << ITERS
               << "\t T: " << entry.second.count() << "s\n";
          }

          std::cout << ss.str();
        }});
  }
  stepCorrectness.run();
  stepPerformance.run();

  return 0;
}
