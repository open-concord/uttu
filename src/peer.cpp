#include "../inc/uttu.hpp"

int Peer::Socket() {
  return this->sockfd;
}

bool Peer::Local() {
  return this->local;
}

bool Peer::Host() {
  return this->host;
}

Peer::Peer(
  int sock,
  struct sockaddr_in socka,
  bool local,
  bool host
) : sec(dhms()), sockfd(sock), sockaddr(socka), local(local), host(host) {};

void Peer::Key_Exchange() {
  // do key exchange [FC]
  try {
    json fco;
    fco["FLAG"] = "KE";
    fco["CONT"] = this->sec.Public();

    this->Raw_Write(fco.dump(), this->tout);
    std::string rs = this->Raw_Read(this->tout);
    auto fci = json::parse(rs);

    if (fci["FLAG"] != "KE") {errc("COULD NOT COMPLETE FC");}
    this->sec.Peer(fci["CONT"].get<std::string>());
    this->sec.Gen();
  } catch (...) {
    errc("Peer::Start, Could not complete Key Exchange");
  }
}

std::string Peer::Raw_Read(unsigned int t) {
  /** read size header */
  char sh[4];
  Timeout to(t, this->Socket());
  bzero(sh, 4); // zero out buffer
  if (read(this->Socket(), &sh, 4) < 0) {
    to.Cancel();
    errc("COULD NOT READ");
  }

  int s = atoi(sh)+4;
  char b[s];
  /** read message */
  bzero(b, s); // zero out buffer
  if (read(this->Socket(), &b, s) < 0) {
    to.Cancel();
    errc("COULD NOT READ");
  }
  to.Cancel();
  return std::string(b);
}

void Peer::Raw_Write(std::string m, unsigned int t) {
  /** 4 dec place size header */
  std::string so = std::to_string(m.size());
  while (so.size()<4) {
    so.insert(0, "0");
  }
  std::string o = so+m;

  Timeout to(t, this->Socket());
  if (write(this->Socket(), o.data(), o.size()) < 0) {
    to.Cancel();
    errc("COULD NOT WRITE");
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
