#include "../../inc/uttu.hpp"

Session Create_socket(std::optional<const char*> target_ip, unsigned short port, unsigned short queue_limit=5) {
  // TODO: actually check system max
  if(queue_limit>=5) {error("QUEUE_LIMIT MUST BE LESS THAN OR EQUAL TO 5");} // [ERROR]
  // initial setup
  session s = new Session(port, queue_limit);
  struct sockaddr_in _self;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
  if(sockfd<0) {error("COULD NOT OPEN");} // [ERROR]
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
  ) <0) {error("COULD NOT BIND");} // [ERROR]

  listen(sockfd, queue_limit);
  s.sockfd = sockfd;
  s.sockaddr = _self;
  return s;
}

Peer* Session::Acceptor(bool(*criteria)(const char*)) {
  while(!this->close) {
    struct sockaddr_in _peer;
    socklen_t _peerlen;
    int _peerfd = accept(
      this->sockfd,
      (struct sockaddr*) &_peer,
      &_peerlen
    );
    if (_peerfd<0) {error("COULD NOT ACCEPT");} // [ERROR]
    if (criteria(_peer->sin_addr)) {
      Peer p = new Peer(_peerfd, _peer);
      return p;
    };
    continue;
  }
  close(this->sockfd);
  this.~Session();
}

void Session::Close() {
  this->close = true;
}

void Peer::Set_Handler(char* (*hnd)(Peer*)) {
  this->handler = hnd;
}

void Peer::Start() {
  int n; // error status
  bzero(this->buff, 256); // zero out buffer
  n = read(this->sockfd, this->buff, 255);
  if (n<0) {error("COULD NOT READ");} // [ERROR]
  char* ret = this->handler(this);
  n = write(this->sockfd, ret, sizeof(ret));
  if (n<0) {error("COULD NOT WRITE");} // [ERROR]
  if (this->done) {close(this->sockfd);}
  this.~Peer();
}
