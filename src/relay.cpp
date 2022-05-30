#include "../inc/uttu.hpp"
#include "../proto/csp/inc/csp.hpp"

void Relay::_Lazy(unsigned int life) {
  try {
    while(!this->close) {
      struct pollfd pfds[1];
      pfds[0].fd = this->net->socketfd();
      pfds[0].events = POLLIN; /** man pages poll(2) has the bit mask values */
      poll(pfds, 1, life);
      if (pfds[0].revents == POLLIN) {
        this->Foward(this->logic);
      } else {
        break;
      }
      continue;
    }
    throw;
  } catch (...) {
    /** destroy poll event please ~u*/
    return;
  }
}

void Relay::Criteria(std::function<bool(std::string)> c = nullptr) {
  this->_c = c;
}

void Relay::Lazy(bool blocking, unsigned int life) {
  if (blocking) {
    this->_Lazy(life);
  } else {
    std::jthread lt(&Relay::_Lazy, this, life);
    lt.detach();
  }
}

void Relay::Foward(std::function<void(Peer*)> l) {
	/** TODO: there needs to be a way to determine targetted protocol, for now, it's just assumed csp */
	csp _n;
  int t = 3000;
	/** pulll peer's connection from own queue */
  try {
    _n.queue(this->net->socketfd());
  } catch(...) {
    std::cout << "[Relay::Foward] Could not _n.queue()\n";
    /** couldn't accept (i wonder why ...) */
  }
	if (this->_c == nullptr || this->_c(_n.peer_ip())) {
		/** breakpoint */ std::cout << "before create\n";
    Peer p(&_n, t, l);
    /** trigger logic manually */
    std::jthread pt(&Peer::_Wake, p);
		/** breakpoint */ std::cout << "after create\n";
		pt.detach();
	}
}

void Relay::Open() {
  listen(this->net->socketfd(), this->queueL);
}

Relay::Relay(
  std::optional<np*> _net,
  unsigned short int r_port,
  unsigned int timeout,
  unsigned short _queueL
) : Peer(_net, timeout), queueL(_queueL) {
  this->net->port(r_port);
}
