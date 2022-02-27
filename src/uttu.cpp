#include "../inc/uttu.hpp"

/** ==non-member functions== */
void Kill_Socket(int s) {
  // do stuff here
  printf("%s\n", "Killing Socket");
  // don't do stuff here
  if (shutdown(s, 2) < 0) {
    errc("COULD NOT CLOSE TIMED OUT SOCKET");
  }
}

void errc(std::string e) {
  std::cout << "[ERROR]" << e << "\n";
  /** exit() causes hanging timeouts */
  return;
}
