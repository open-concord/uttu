/** concord standard (network) protocol */
#include "../inc/uttu.hpp"

struct sockaddr_in np::_form(int port) {
  struct sockaddr_in _peer;

  int _peerfd = socket(this->cfamily, SOCK_STREAM, 0);
  if (_peerfd < 0) {
    errc("[NP::FORM] COULD NOT CREATE SOCKET");
  }

  bzero((char*) &_peer, sizeof(_peer));

  _peer.sin_family = AF_INET;
  if (port > 0) {_peer.sin_port = htons(port);}

  this->sockfd = _peerfd;
  return _peer;
}

void np::target(np::_tf target) {
  struct hostent* s;

  s = gethostbyname(target.addr.c_str());
  if (s == NULL) {
    errc("[NP::TARGET] COULD NOT FIND PEER's ADDR");
  }
  struct sockaddr_in _peer = this->_form(target.port);

  bcopy(
    (char*)s->h_addr,
    (char*)&_peer.sin_addr.s_addr,
    s->h_length
  );

  if (connect(
    this->sockfd,
    (struct sockaddr*) &_peer,
    (socklen_t) sizeof(_peer)
  ) < 0) {
    errc("[NP::TARGET] COULD NOT CONNECT");
  };
  /** TODO: store _peer */
  this->self = _peer;
}

void np::queue(int origin_fd) {
  struct sockaddr_in _in;
  socklen_t _inlen = sizeof(_in);
  int fd = accept(origin_fd, (struct sockaddr *) &_in, &_inlen);
  if (fd < 0) {
    errc("[NP::QUEUE] COULD NOT ACCEPT");
  } else {
    this->sockfd = fd;
    this->self = _in;
  }
}

void np::port(unsigned short int port) {
  struct sockaddr_in _info = this->_form(port);
  
  if (bind(this->sockfd, (struct sockaddr* ) &_info, sizeof(_info)) < 0) {
    errc("[NP::PORT] COULD NOT BIND");
  }
 
  /** TODO: store _info */
  this->self = _info;
}

int np::socketfd() {
  return this->sockfd;
}

std::string np::peer_ip() {
  char ip[INET_ADDRSTRLEN];
  std::string str_ip = inet_ntop(this->cfamily, &(this->self.sin_addr), ip, INET_ADDRSTRLEN);
  return str_ip;
}

/** read (no status) */
std::string np::readb() {
  /** read size header */
  char sh[4];
  bzero(sh, 4); // zero out buffer
  if (read(this->sockfd, &sh, 4) < 0) {
    errc("[NP::READB] COULD NOT READ SIZE HEADER");
  }

  int s = atoi(sh)+4;
  char b[s];
  /** read message */
  bzero(b, s); // zero out buffer
  if (read(this->sockfd, &b, s) < 0) {
    errc("[NP::READB] COULD NOT READ CONTENT BUFFER");
  }

  if (strlen(b) < 1) {
    errc("[NP::READB] ZERO-LENGTH MESSAGE");
  }

  return std::string(b);
}

/** write (no status) */
void np::writeb(std::string m) {
  if (m.length() < 1) {
    errc("[NP::WRITEB] ZERO-LENGTH MESSAGE");
  }
  /** 4 dec place size header */
  std::string so = std::to_string(m.size());
  while (so.size()<4) {
    so.insert(0, "0");
  }
  std::string o = so+m;
  if (write(this->sockfd, o.data(), o.size()) < 0) {
    errc("[NP::WRITEB] COULD NOT WRITE");
  };
}

/** close (no status) */
void np::closeb() {
  close(this->sockfd);
}

np::np() {};
