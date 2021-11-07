#include "../inc/uttu.hpp"

int Session::Socket() {
  return this->sockfd;
}

void Session::Criteria(bool(*criteria)(std::string)) {
  this->_criteria = criteria;
}

Session::Session(
  unsigned short port,
  unsigned short queue_limit,
  int sockfd,
  struct sockaddr_in _self
) : port(port),
    queue_limit(queue_limit),
    sockfd(sockfd),
    sockaddr(_self)
{};

std::shared_ptr<Peer> Session::Accept() {
  if(this->close) {this->Close();}

  struct sockaddr_in _peer;
  socklen_t _peerlen;

  // start timeout
  Timeout to(3000, this->Socket());
  int _peerfd = accept(
    this->sockfd,
    (struct sockaddr*) &_peer,
    &_peerlen
  );
  to.Cancel();

  struct sockaddr_in pinfo;
  socklen_t pinfo_len = sizeof(pinfo);

  if (_peerfd < 0) {
    errc("COULD NOT ACCEPT");
  }
  if (getpeername(_peerfd, (struct sockaddr *) &pinfo, &pinfo_len) < 0) {
    errc("COULD NOT GETPEERNAME");
  }

  char ip[INET_ADDRSTRLEN]; // some macro for addr len
  inet_ntop(2 /** AF INET */, &(pinfo.sin_addr), ip, INET_ADDRSTRLEN);

  if (this->_criteria(ip)) {
    bool f = false;
    // local checking
    std::string ips(ip);
    if (ips == "127.0.0.1" || ips == "::1") {f = true;}
    auto p = std::make_shared<Peer> (_peerfd, _peer, f);
    return p;
  }

  if (shutdown(_peerfd, 2) < 0) {
    errc("COULD NOT CLOSE REJECTED PEER SOCKET");
  }
}

std::shared_ptr<Peer> Session::Connect(std::string ip, unsigned int port) {
  if(this->close) {this->Close();}

  struct sockaddr_in _peer;

  _peer.sin_family = AF_INET;
  _peer.sin_port = htons(port);

  if (inet_pton(AF_INET, ip.c_str(), &_peer.sin_addr) < 0) {
    errc("COULD NOT PTON TARGET IP");
  }

  // start timeout
  Timeout to(3000, this->Socket());
  int _peerfd = connect(
    this->sockfd,
    (struct sockaddr*) &_peer,
    sizeof(_peer)
  );
  to.Cancel();

  bool f;
  (ip == "127.0.0.1") ? f = true : f = false;
  auto p = std::make_shared<Peer>(_peerfd, _peer, f);
  return p;
}

void Session::Close() {
  this->close = true;
  if (shutdown(this->sockfd, 0) < 0) {
    errc("COULD NOT CLOSE SOCKET");
  }
  this->~Session();
}
