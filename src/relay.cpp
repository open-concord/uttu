#include "../inc/uttu.hpp"

void Relay::_Lazy(unsigned int life) {
  try {
    while(!this->close) {
      struct pollfd pfds[1];
      pfds[0].fd = this->net.socketfd();
      pfds[0].events = POLLIN; /** man pages poll(2) has the bit mask values */
      poll(pfds, 1, life);
      if (pfds[0].revents == POLLIN) {
        this->Foward(this->l);
      } else {
        break;
      }
      continue;
    }
    throw;
  } catch (...) {
    /** destory pollevent please ~u*/
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
    std::thread lt(&Relay::_Lazy, this, life);
    lt.detach();
  }
}

void Relay::Foward(std::function<void(Peer*)> l) {
	Timeout to(this->tout, this->net.socketfd());
	/** TODO: there needs to be a way to determine targetted protocol, for now, it's just assumed csp */
	np _n;
  int t = 3000;
	/** pulll peer's connection from own queue */
	_n.queue(this->net.socketfd());
	to.Cancel();
	if (this->_c == nullptr || this->_c(_n.peer_ip())) {
		/** breakpoint */ std::cout << "before create\n";
    
    /** FAILS: std::thread np (
        [_n, t, l]() -> Peer {
          return Peer(_n, t, l);
        });
    */
		/** breakpoint */ std::cout << "after create\n";
		//np.detach();
	}
}

void Relay::Load(std::function<void(Peer*)> l) {
  this->logic = l;
}

void Relay::Open() {
  listen(this->net.socketfd(), this->queueL);
}

Relay::Relay(
  np _net,
  unsigned short int r_port,
  unsigned int timeout,
  unsigned short _queueL
) : Peer(_net, timeout), queueL(_queueL) {
  this->net.port(r_port);
}
