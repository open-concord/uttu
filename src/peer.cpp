 #include "../inc/uttu.hpp"

/** == gets == */

bool Peer::Host() {
  return this->host;
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

/**
Peer::Peer() {
  // TODO: actually check system max
  if(queue_limit > 5) {errc("QUEUE_LIMIT MUST BE LESS THAN OR EQUAL TO 5");}
  // initial setup
  struct sockaddr_in _self;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
  if(sockfd<0) {errc("COULD NOT OPEN");}
  bzero((char*) &_self, sizeof(_self)); // zeroing out self_addr buffer

  // sockaddr config(s)
  _self.sin_family = AF_INET;
  _self.sin_addr.s_addr = INADDR_ANY;
  _self.sin_port = htons(port); // made sure to convert to net byte order

  // binding socket
  if (bind(
    sockfd,
    (struct sockaddr*) &_self,
    sizeof(_self)
  ) <0) {errc("COULD NOT BIND");} // [ERROR]
}
*/
