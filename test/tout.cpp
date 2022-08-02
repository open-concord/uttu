#include "../inc/uttu.hpp"
#include <cassert>
#include <fcntl.h>

int main(void) {
  std::atomic<bool> a{false};
  
  int sid = socket(AF_LOCAL, SOCK_STREAM, 0);
  assert(sid > 0);
  Timeout t(3000, sid);
  t.Suspend(a);
  int i = 3;
  while (!a && i>0) {
    // still open?
    assert(fcntl(sid, F_GETFD) > -1);
    if (i > 1) {
      i--;
    } else {
      a = true;
      a.notify_all();
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  std::cout << "[%] now closing\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));
  /** did timeout finally close? */
  assert(fcntl(sid, F_GETFD) < 0);
  return 0;
}
