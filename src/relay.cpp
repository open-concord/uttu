#include "../inc/relay.hpp"
#include "../inc/debug.hpp"
#include <iostream>
#include <thread>
#include <stdexcept>

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

void Relay::_Lazy(unsigned int life) {
  while(!Flags.Get(Relay::CLOSE, 1) || Flags.Get(Relay::LAZY, 1)) {
    struct pollfd pfds[1];
    pfds[0].fd = this->net.fd();
    pfds[0].events = POLLIN; // man pages poll(2) has the bit mask values
    poll(pfds, 1, life); 
    
    bool cont = false;
    switch(pfds[0].revents) {
      case POLLRDNORM: // equivalent to POLLIN
      case POLLIN:
        this->Foward();
        break;
      case POLLPRI:
        debug.bump("- [!!] POLLPRI hit");
        break;
      case POLLRDHUP:
      case POLLHUP:
        debug.bump("- [%] POLLHUP hit");
        cont = true;
        break;         
      case POLLERR:
        debug.bump("- [%] Poll Error");
        break;
      case POLLNVAL:
        debug.bump("- [%] Invalid FD"); 
        break;
      default:
        cont = true;
    }
    if (cont) {
      debug.bump("L [%] Restarting Poll");
      continue;
    } else {
      debug.bump("L [!] Closing Poll");
    }
  }
}

void Relay::Embed(std::function<void(Peer&&)> e) {
  this->_e = e;
}

void Relay::Lazy(bool blocking, unsigned int life) {
  if (Flags.Get(Relay::LAZY, 1)) {return;}
  Flags.Set(Relay::LAZY, true, 1);
  if (blocking) {
    this->_Lazy(life);
  } else {
    std::jthread lt(&Relay::_Lazy, this, life);
    lt.detach();
  }
}

void Relay::Foward() {	
	int t = 3000;
  /** create peer */
  np& pnp = std::reference_wrapper<np>(net);
  Peer&& p = Peer(pnp, t);
  p.Flags.Set(Peer::HOST, true);
  
  /** pull peer's connection from own queue */
  p.net.queue(this->net.fd());

  this->_e(std::move(p));
}

void Relay::Open() {
  if (Flags.Get(Relay::OPEN, 1)) {return;}
  try {
    listen(this->net.fd(), this->queueL);
    Flags.Set(Relay::OPEN, true, 1);
  } catch (std::exception& e) {
    debug.bump("[!] ", e.what());
  }
}

void Relay::Close() {
  this->Flags.Set(Relay::CLOSE, true, 1);
  this->net.closeb();
}

Relay::Relay(
  np& _net,
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
