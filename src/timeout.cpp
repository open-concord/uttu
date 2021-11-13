#include "../inc/uttu.hpp"

// private
void Timeout::_async(int t) {
  // v there's probably a better way to do this
  std::this_thread::sleep_for(std::chrono::milliseconds(t));
  try {
    if (this->_cancel) {
      throw;
    } {
      Kill_Socket(this->sk);
    }
  } catch (...) {
    /** either Timeout no longer exists and or is cancelled */
    return;
  }
}

// public
Timeout::Timeout(unsigned int t, int s) {
  this->sk = s;
  this->expire = std::thread(&Timeout::_async, this, t);
  this->expire.detach();
}

void Timeout::Cancel() {
  this->_cancel = true;
}
