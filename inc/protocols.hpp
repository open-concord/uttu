#pragma once

#include <string>

#ifdef __linux__ // linux
#include <sys/socket.h> // duh
#include <sys/types.h> // needed by socket.h
#include <netinet/in.h> // internet socket protocol
#include <strings.h> // bzero
#include <unistd.h> // close, accept, etc
#include <arpa/inet.h> // needed for ip ID
#include <netdb.h> // gethostbyname
#include <poll.h> // ppoll & poll
#endif

/** extendable formatting */
struct np {
  int sockfd = -1;

  /** origin */ 
  virtual void port(unsigned short int _port) = 0;
  virtual void queue(int origin_fd) = 0;

  /** operations */
  virtual std::string readb() = 0;
  virtual void writeb(std::string m) = 0;
  virtual void closeb() = 0;

  /** status */
  virtual int fd() = 0;
  
  /** c/d */
  virtual ~np() = default;
};
