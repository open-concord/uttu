#include "../inc/uttu.hpp"
#include "../proto/csp/inc/csp.hpp"

/** == status == */

bool Peer::Host() {
  return this->Flags.GetFlag(Peer::HOST);
}

void Peer::Port(unsigned int p) {
  this->net->port(p);
}

/** == raw r/w (these should not be called) == */
std::string Peer::Raw_Read(unsigned int t) {
  if (t < 1) {t = this->tout;}

  Timeout to(t, this->net->socketfd());
  std::string m = this->net->readb();
  to.Cancel();
  return m;
}

void Peer::Raw_Write(std::string m, unsigned int t) {
  if (t < 1) {t = this->tout;}

  Timeout to(t, this->net->socketfd());
  this->net->writeb(m);
  to.Cancel();
}

/** == abstract r/w  (these should be called) == */
std::string Peer::Read(unsigned int t) {
  if (t < 1) {t = this->tout;}
  
  return this->sec.AD(this->Raw_Read(t));
}

void Peer::Write(std::string m, unsigned int t) {
  if (t < 1) {t = this->tout;}

  this->Raw_Write(this->sec.AE(m), t);
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

/** WIP: Make Peer bootstrappable */
Peer::Peer(
  std::optional<np*> _net,
  unsigned int timeout
) : Flags(UFTEMP::Peer), tout(timeout) {
  Flags.SetFlag(Peer::CLOSE, false);
  if (!_net.has_value()) {
    std::cout << "[%] No Protocol Passed, assuming CSP\n"; // DEBUG
    /** csp */
    this->net = new csp;
  } else {this->net = _net.value();} 
}

