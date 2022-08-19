#pragma once

#include <string>

// this feels like it restricts possible
// future protocol forms;
// we should move away from AFNET centric
// signatures;
// -- possible steps
// 1. deliberatly create a layer of abstraction between
// - protocol i/o and actual functionality
// 2. refine target formatting to reduce unnessacry complexity
// - in protocol structure

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
protected:
  struct sockaddr_in self = {};
  int cfamily = AF_INET;
  int sockfd = -1;
public:
  struct _tf {
    std::string addr;
    unsigned short int port;
  } tf; // target format

  /** form socket */ 
  virtual struct sockaddr_in _form(int port = -1) = 0;

  /** origin */
  virtual void target(_tf t) = 0;
  virtual void port(unsigned short int _port) = 0;
  virtual void queue(int origin_fd) = 0;

  /** operations */
  virtual std::string readb() = 0;
  virtual void writeb(std::string m) = 0;
  virtual void closeb() = 0;

  /** status */
  virtual int socketfd() = 0;
  virtual std::string peer_ip() = 0;
  
  virtual ~np() = default;
};
