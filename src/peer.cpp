 #include "../inc/uttu.hpp"

/** == status == */

bool Peer::Host() {
  return this->host;
}

void Peer::_Wake() {
  this->logic(this);
}

void Peer::Swap(std::function<void(Peer*)> l) {
  this->logic = l;
}

/** == raw r/w (these should not be called) == */
std::string Peer::Raw_Read(unsigned int t) {
  Timeout to(t, this->net.socketfd());
  std::string m = this->net.readb();
  to.Cancel();
  return m;
}

void Peer::Raw_Write(std::string m, unsigned int t) {
  Timeout to(t, this->net.socketfd());
  this->net.writeb(m);
  to.Cancel();
}

/** == abstract r/w  (these should be called) == */
std::string Peer::Read(unsigned int t) {
  return this->sec.AD(this->Raw_Read(t));
}

void Peer::Write(std::string m, unsigned int t) {
  this->Raw_Write(this->sec.AE(m), t);
}

void Peer::Close() {
  this->net.closeb();
}

void Peer::Connect(std::string target) {
  /** input parse */
  unsigned short p = atoi(target.substr((target.find(":")+1)).data()); // delimter->end;
  std::string a = target.substr(0, target.find(":")).data();

  /** change socket target */
  Timeout th(this->tout, this->net.socketfd());
  /** TODO: change based on net protocol */
  this->net.target(
    np::_tf {a, p}
  );
  th.Cancel();
}

/** WIP: Make Peer bootstrappable */

/** outgoing initial peer */
Peer::Peer(
  np _net,
  unsigned short int r_port,
  unsigned int timeout,
  std::function<void(Peer*)> l
) : net(_net), host(false), tout(timeout), logic(l) {
  this->net.port(r_port);
}

/** incoming initial peer */
Peer::Peer(
  np _net,
  unsigned int timeout,
  std::function<void(Peer*)> l
) : net(_net), host(true), tout(timeout), logic(l) {

}
