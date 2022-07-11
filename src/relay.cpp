#include "../inc/uttu.hpp"
#include "../proto/csp/inc/csp.hpp"

void Relay::_Lazy(unsigned int life) {
  try {
    while(!Flags.Get(Relay::CLOSE, 1)) {
      struct pollfd pfds[1];
      pfds[0].fd = this->net->socketfd();
      pfds[0].events = POLLIN; /** man pages poll(2) has the bit mask values */
      poll(pfds, 1, life);
      if (pfds[0].revents == POLLIN) {
        this->Foward();
      } else {
        break;
      }
      continue;
    }
    throw "";
  } catch(const char* m) {
    std::cout << "[%] Stopping Lazy Accept\n";
  } catch (std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
    /** destroy poll event please ~u*/
    return;
  }
}

void Relay::Embed(std::function<void(std::unique_ptr<Peer>)> e) {
  this->_e = e;
}

void Relay::Criteria(std::function<bool(std::string)> c) {
  this->_c = c;
}

void Relay::Lazy(bool blocking, unsigned int life) {
  if (Flags.Get(Relay::LAZY, 1)) {return;}
  if (blocking) {
    this->_Lazy(life);
  } else {
    std::jthread lt(&Relay::_Lazy, this, life);
    lt.detach();
  }
  Flags.Set(Relay::LAZY, true, 1);
}

void Relay::Foward() {	
	int t = 3000;
  /** create peer */
  Peer p(new csp, t); 
  p.Flags.Set(Peer::UNTRUSTED, true);
  p.Flags.Set(Peer::HOST, true);
  /** pull peer's connection from own queue */
  p.net->queue(this->net->socketfd());

	if (this->_c == nullptr || this->_c(p.net->peer_ip())) {
		p.Flags.Set(Peer::UNTRUSTED, false);
    this->_e(std::make_unique<Peer>(p));
	}
}

void Relay::Open() {
  if (Flags.Get(1, Relay::OPEN)) {return;}
  try {
    listen(this->net->socketfd(), this->queueL);
    Flags.Set(1, Relay::OPEN, true);
  } catch (std::exception& e) {
    std::cout << "[!] " << e.what() << '\n';
  }
}

void Relay::Close() {
  this->Flags.Set(Relay::CLOSE, true, 1);
  this->net->closeb();
}

Relay::Relay(
  std::optional<np*> _net,
  unsigned short int r_port,
  unsigned int timeout,
  unsigned short _queueL
) : Peer(_net, timeout), queueL(_queueL) {  
  this->Port(r_port);
  this->Flags.Reserve(0, 4);
  this->Flags.Fill(false);
  this->Flags.Reserve(1, 3);
  this->Flags.Fill(false, 1);
  this->Flags.Set(Peer::HOST, true); 
}
