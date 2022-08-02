#include "../inc/uttu.hpp"

/** ==non-member functions== */
void Kill_Socket(int s) {
  // do stuff here
  printf("%s\n", "[%] Killing Socket");
  // don't do stuff here
  int e = close(s);
  if (e < 0) {
    int c = errno;
    if (c == 9) {return;} // already closed
    else {
      errc("[!!] ErrNo "+c);
    }
  }
}

void errc(std::string e) {
  std::cout << "[ERROR]" << e << "\n";
  /** exit() causes hanging timeouts */
  return;
}
