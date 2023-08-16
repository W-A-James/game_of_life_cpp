#include "life.h"
#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <thread>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

static void end(int sig);

int main(int argc, char **argv) {
  using namespace std::chrono_literals;
  // Sim state
  bool running{true};
  auto frame_time_ms = 30ms;
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
  auto life = new Life(columns, rows, 0.6);

  initscr();
  keypad(stdscr, TRUE);
  nonl();
  cbreak();
  nodelay(stdscr, TRUE);

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
  }

  std::chrono::time_point<std::chrono::steady_clock> previous =
      std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> now =
      std::chrono::steady_clock::now();

  for (;;) {
    auto time_since_previous_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - previous);

    if (running && (time_since_previous_ms.count() >= frame_time_ms.count())) {
      int y{0}, x{0};
      move(0, 0);
      life->step();
      std::string s = life->toString();
      for (auto c : s) {
        switch (c) {
        case '\n':
          y++;
          x = 0;
          break;
        default:
          auto alive = c == ALIVE;
          attrset(COLOR_PAIR((int)alive + 1));
          addch('X');
          x++;
          break;
        }
      }
      previous = now;
    }

    attrset(COLOR_PAIR(0));
    mvprintw(0, 0, "paused: %s ", running ? "false" : "true");
    mvprintw(1, 0, "frame time: %lu ms", frame_time_ms.count());

    refresh();

    char pressed_key;
    pressed_key = getch();
    switch (pressed_key) {
    case ERR:
      break;
    case 'r':
      life = new Life(columns, rows, 0.6);
      break;
    case 'j':
      frame_time_ms -= 10ms;
      if (frame_time_ms <= 0ms) {
        frame_time_ms = 0ms;
      }
      break;
    case 'k':
      frame_time_ms += 10ms;
      if (frame_time_ms >= 200ms) {
        frame_time_ms = 200ms;
      }
      break;
    case ' ':
      running = !running;
      break;
    }

    now = std::chrono::steady_clock::now();
  }

  end(0);
}

static void end(int sig) {
  endwin();
  exit(0);
}
