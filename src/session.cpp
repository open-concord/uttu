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

void Session::Open() {
  listen(this->sockfd, this->queue_limit);
}

std::shared_ptr<Peer> Session::Accept() {
  if(this->close) {this->Close();}

  struct sockaddr_in _peer;
  socklen_t _peerlen;

  // start possibly hanging accept
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
    auto p = std::make_shared<Peer> (_peerfd, _peer, f, true);
    return p;
  }

  if (shutdown(_peerfd, 2) < 0) {
    errc("COULD NOT CLOSE REJECTED PEER SOCKET");
  }
}

std::shared_ptr<Peer> Session::Connect(std::string target /** host:port */) {
  if(this->close) {this->Close();}

  struct sockaddr_in _peer; // peer addr
  struct hostent* s;

  int port = atoi(target.substr((target.find(":")+1)).data()); // delimter->end
  int _peerfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_peerfd < 0) {
    errc("[CONNECT], COULD NOT CREATE SOCKET");
  }

  Timeout th(3000, this->Socket());
  s = gethostbyname(target.substr(0, target.find(":")).data());
  if (s == NULL) {
    errc("[CONNECT], COULD NOT FIND PEER");
  }
  th.Cancel();
  bzero((char*) &_peer, sizeof(_peer));

  _peer.sin_family = AF_INET;
  _peer.sin_port = htons(port);
  bcopy(
    (char*)s->h_addr,
    (char*)&_peer.sin_addr.s_addr,
    s->h_length
  );

  // start timeout
  Timeout to(3000, this->Socket());
  if (connect(
    _peerfd,
    (struct sockaddr*) &_peer,
    (socklen_t) sizeof(_peer)
  ) < 0) {
    errc("[CONNECT], COULD NOT CONNECT");
  };
  to.Cancel();

  bool f;
  (target.substr(0, target.find(":")) == "127.0.0.1") ? f = true : f = false;
  auto p = std::make_shared<Peer>(_peerfd, _peer, f, false);
  return p;
}

void Session::_Lazy(void(*h)(std::shared_ptr<Peer>)) {
  /** put this in a thread */
  try {
    while(!this->close) {
      struct pollfd pf;
      pf.fd = this->Socket();
      pf.events = POLLIN; /** man pages poll(2) has the bit mask values */

      switch (poll(&pf, 1, 3000)) {
        case -1:
          errc("POLL RETURNED -1");
        case 0:
          h(this->Accept());
          continue;
      }
    }
    throw;
  } catch (...) {
    return;
  }

}

void Session::Lazy(void(*h)(std::shared_ptr<Peer>)) {
  /** put this in a thread */
  std::thread lt(&Session::_Lazy, this, h);
  lt.detach();
}

void Session::Close() {
  this->close = true;
  if (shutdown(this->sockfd, 0) < 0) {
    errc("COULD NOT CLOSE SOCKET");
  }
  this->~Session();
}
