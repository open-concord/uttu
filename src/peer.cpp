#include "../inc/peer.hpp"
#include "../inc/timeout.hpp"
#include "../inc/debug.hpp"

#include <iostream>

/** == status == */

bool Peer::Host() {
  return this->Flags.Get(Peer::HOST);
}

void Peer::Port(unsigned int p) {
  this->net.port(p);
}

/** == raw r/w (these should not be called) == */
std::string Peer::RawRead(unsigned int t) {
  if (t < 1) {t = this->tout;}

  Timeout to(t, this->net.fd());
  std::string m = this->net.readb();
  to.Cancel();
  return m;
}

std::string Peer::AwaitRawRead(unsigned int l) {
  int life = l;
  if (l < 1) {life = -1;}
  try {
    while(!this->Flags.Get(Peer::CLOSE)) {
      struct pollfd pfds[1];
      pfds[0].fd = this->net.fd();
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

  Timeout to(t, this->net.fd());
  this->net.writeb(m);
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
  this->net.closeb();
}

Peer::Peer(
  np& _net,
  unsigned int timeout
) : net(_net), tout(timeout) {
  Flags.Reserve(0, 4);
  Flags.Fill(false);
}

