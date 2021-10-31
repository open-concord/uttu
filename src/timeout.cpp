#include "../inc/uttu.hpp"

// private
void Timeout::_async(int ft) {
  // v there's probably a better way to do this
  while(std::time(nullptr) < ft) {
    if (this->_cancel) {return;}
  }
  Kill_Socket(this->sk); // kill socket
}

// public
Timeout::Timeout(unsigned int t, int s) {
  this->sk = s;
  this->await = std::thread(&Timeout::_async, this, (std::time(nullptr)+t));
}

void Timeout::Cancel() {
  this->_cancel = true;
  this->await.join();
}
