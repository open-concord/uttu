#include "../inc/uttu.hpp"

int Peer::Socket() {
  return this->sockfd;
}

bool Peer::Local() {
  return this->local;
}

Peer::Peer(
  int sock,
  struct sockaddr_in socka,
  bool local
) : sockfd(sock), sockaddr(socka), local(local), sec(dhms()) {};

void Peer::Start(void (*h)(Peer*)) {
  // get peer pub [FC]
  
  h(this);
}

std::string Peer::Read(unsigned int t) {
  char* b = new char[1024]; // maybe set this to be configurable?
  Timeout to(t, this->Socket());
  bzero(b, sizeof(b)); // zero out buffer
  if (read(this->Socket() /** for standardization */, b, (sizeof(b)-1)) < 0) {
    errc("COULD NOT READ");
    to.Cancel();
    return nullptr;
  }
  to.Cancel();
  return std::string(b);
}

void Peer::Write(std::string m, unsigned int t) {
  Timeout to(t, this->Socket());
  if (send(this->Socket()/** for standardization */, m.c_str(), m.size(), 0) < 0) {
    errc("COULD NOT WRITE");
    to.Cancel();
    return;
  };
  to.Cancel();
}

void Peer::Close() {
  shutdown(this->Socket(), 2);
}
