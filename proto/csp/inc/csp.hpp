/** concord standard (network) protocol */
#pragma once
#include "../../../inc/uttu.hpp"

struct csp : public np {
  struct sockaddr_in _form(int port) override;
  void target(_tf target) override;
  void queue(int origin_fd) override;
  void port(unsigned short int port) override;
  int socketfd() override;
  std::string peer_ip() override;
  /** read (no status) */
  std::string readb() override;
  /** write (no status) */
  void writeb(std::string m) override;
  /** close (no status) */
  void closeb() override;
  ~csp() {}
};
