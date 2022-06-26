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
  if (this->Flags.Lazy) {return;}
  if (blocking) {
    this->_Lazy(life);
  } else {
    std::jthread lt(&Relay::_Lazy, this, life);
    lt.detach();
  }
  this->Flags.Lazy = true;
}

void Relay::Foward(std::function<void(Peer*)> l) {
	/** TODO: there needs to be a way to determine targetted protocol, for now, it's just assumed csp */
	int t = 3000;
  /** create peer */
  Peer p(new csp, t, l);
  p.SetFlag(UNTRUSTED, true);
  /** pull peer's connection from own queue */
  p.net->queue(this->net->socketfd());
 
	if (this->_c == nullptr || this->_c(p.net->peer_ip())) {
		p.SetFlag(UNTRUSTED, false);
    /** trigger logic manually */
    std::jthread pt(&Peer::_Wake, p);
		pt.detach();
	}
}

void Relay::Open() {
  if (this->Flags.Open) {return;}
  try {
    listen(this->net->socketfd(), this->queueL);
    this->Flags.Open = true;
  } catch (std::exception& e) {
    std::cout << "[!] " << e.what() << '\n';
  }
}

Relay::Relay(
  std::optional<np*> _net,
  unsigned short int r_port,
  unsigned int timeout,
  unsigned short _queueL
) : queueL(_queueL), Peer(_net, timeout), FlagManager(3) {
  this->Port(r_port);
  this->host = true; 
}
