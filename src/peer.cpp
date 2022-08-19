#include "../inc/peer.hpp"
#include "../inc/timeout.hpp"
#include "../proto/csp/inc/csp.hpp"
#include "../inc/debug.hpp"

#include <iostream>

/** == status == */

bool Peer::Host() {
  return this->Flags.Get(Peer::HOST);
}

void Peer::Port(unsigned int p) {
  this->net->port(p);
}

/** == raw r/w (these should not be called) == */
std::string Peer::RawRead(unsigned int t) {
  if (t < 1) {t = this->tout;}

  Timeout to(t, this->net->socketfd());
  std::string m = this->net->readb();
  to.Cancel();
  return m;
}

std::string Peer::AwaitRawRead(unsigned int l) {
  int life = l;
  if (l < 1) {life = -1;}
  try {
    while(!this->Flags.Get(Peer::CLOSE)) {
      struct pollfd pfds[1];
      pfds[0].fd = this->net->socketfd();
      pfds[0].events = POLLIN;
      poll(pfds, 1, life);
      if (pfds[0].revents == POLLIN) {
        return this->RawRead();
      } else {
        break;
      }
      continue;
    }
    throw;
  } catch (...) {
    // TODO destroy poll event
    return "";
  }
}

void Peer::RawWrite(std::string m, unsigned int t) {
  if (t < 1) {t = this->tout;}

  Timeout to(t, this->net->socketfd());
  this->net->writeb(m);
  to.Cancel();
}

/** == abstract r/w  (these should be called) == */
std::string Peer::Read(unsigned int t) {
  if (t < 1) {t = this->tout;}
  
  return this->sec.AD(this->RawRead(t));
}

std::string Peer::AwaitRead(unsigned int l) {
  return this->sec.AD(this->AwaitRawRead(l));
}

void Peer::Write(std::string m, unsigned int t) {
  if (t < 1) {t = this->tout;}

  this->RawWrite(this->sec.AE(m), t);
}

void Peer::Close() {
  this->net->closeb();
}

void Peer::Connect(std::string ip, unsigned short int port) {
  /** change socket target */
  Timeout th(this->tout, this->net->socketfd());
  /** TODO: change based on net protocol */
  this->net->target(
    np::_tf {ip, port}
  );
  th.Cancel();
}

Peer::Peer(
  std::optional<np*> _net,
  unsigned int timeout
) : tout(timeout) {
  Flags.Reserve(0, 4);
  Flags.Fill(false);
  if (!_net.has_value()) {
    debug.bump("[%] No Protocol Passed, assuming CSP");
    /** csp */
    this->net = new csp;
  } else {this->net = _net.value();}
}

