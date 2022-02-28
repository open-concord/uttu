#include "../inc/uttu.hpp"

// private
void Timeout::_async(int t) {
  std::this_thread::sleep_for(std::chrono::milliseconds(t));
  try {
    if (this->_cancel) {
      throw;
    } else {
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
  this->expire = std::jthread(&Timeout::_async, this, t);
  this->expire.detach();
}

void Timeout::Cancel() {
  this->_cancel = true;
}
