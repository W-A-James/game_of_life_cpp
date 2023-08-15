#include "neighbour_refs_par.h"
#include <chrono>
#include <iostream>
#include <thread>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

int main(int argc, char **argv) {
  size_t columns;
  size_t rows;
#ifdef WIN32
  CONSOLE_SCREEN_BUFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  columns = w.ws_col;
  rows = w.ws_row;
#endif
  using namespace std::chrono_literals;
  Life life{columns, rows, 0.4};

  while (true) {
    int res = system("clear");
    life.step();
    std::cout << life.toString();
    std::this_thread::sleep_for(60ms);
  }

  return 0;
}
