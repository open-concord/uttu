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
) : sockfd(sock), sockaddr(socka), local(local) {};

void Peer::Start(void (*h)(Peer*)) {
  h(this);
}

std::string Peer::Read(unsigned int t) {
  bool d = false;
  char* b = new char[1024]; // maybe set this to be configurable?
  Timeout to(t, &d);
  bzero(b, sizeof(b)); // zero out buffer
  if (read(this->Socket() /** for standardization */, b, (sizeof(b)-1)) < 0 || d == true) {
    errc("COULD NOT READ");
    to.Cancel();
    return nullptr;
  }
  to.Cancel();
  return std::string(b);
}

void Peer::Write(std::string m, unsigned int t) {
  bool d = false;
  Timeout to(t, &d);
  if (send(this->Socket()/** for standardization */, m.c_str(), m.size(), 0) < 0 || d == true) {
    errc("COULD NOT WRITE");
    to.Cancel();
    return;
  };
  to.Cancel();
}

void Peer::Close() {
  shutdown(this->Socket(), 2);
}
