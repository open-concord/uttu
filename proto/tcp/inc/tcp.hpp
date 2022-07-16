#pragma once
#include <uttu.hpp>

struct utcp : public np {
  /** form socket */ 
  struct sockaddr_in _form(int port = -1) override;

  /** origin */
  void target(_tf t) override;
  void port(unsigned short int _port) override;
  void queue(int origin_fd) override;

  /** operations */
  std::string readb() override;
  void writeb(std::string m) override;
  void closeb() override;

  /** status */
  int socketfd() override;
  std::string peer_ip() override;
};
