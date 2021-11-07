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
) : sec(dhms()), sockfd(sock), sockaddr(socka), local(local) {};

void Peer::Start(void (*h)(Peer*)) {
  // get peer pub [FC]
  json fco = {
    {"FC", "KE"},
    {"CONT", this->sec.Public()}
  };
  std::cout << fco.dump() << "\n";
  this->Raw_Write(fco.dump(), 3000);

  auto fci = json::parse(this->Raw_Read(3000));
  std::cout << fci << "\n";
  if (fci["FLAG"] != "KE") {errc("COULD NOT COMPLETE FC");}
  this->sec.Peer(fci["CONT"]);

  h(this);
}

std::string Peer::Raw_Read(unsigned int t) {
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

void Peer::Raw_Write(std::string m, unsigned int t) {
  Timeout to(t, this->Socket());
  if (send(this->Socket()/** for standardization */, m.c_str(), m.size(), 0) < 0) {
    errc("COULD NOT WRITE");
    to.Cancel();
    return;
  };
  to.Cancel();
}

std::string Peer::Read(unsigned int t) {
  return this->sec.AD(this->Raw_Read(t));
}

void Peer::Write(std::string m, unsigned int t) {
  this->Raw_Write(this->sec.AE(m), t);
}

void Peer::Close() {
  shutdown(this->Socket(), 2);
}
