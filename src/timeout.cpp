#include "../inc/timeout.hpp"
#include "../inc/debug.hpp"

// private
void Timeout::_async(int t) { 
  std::this_thread::sleep_for(std::chrono::milliseconds(t));
  try {
    if (this->_cancel) {
      return; 
    } else {
      int c = close(this->sk);
      if (c < 9)
        return;
    }
  } catch (std::exception &e) { // uh oh
    debug.bump("[!] ", e.what());
  }
}

// public
Timeout::Timeout(unsigned int t, int s) {
  this->sk = s;
  this->tout = t;
  this->expire = std::jthread(&Timeout::_async, this, t);
  this->expire.detach();
}

void Timeout::Suspend(
    std::atomic<bool>& reanimate
  ) {
  this->_cancel = true;
  auto await = std::jthread([&]{ 
    reanimate.wait(false);
    this->_cancel = false;
    this->expire = std::jthread(&Timeout::_async, this, this->tout);
    this->expire.detach();
  });
  await.detach();
}

void Timeout::Cancel() {
  this->_cancel = true;
}
