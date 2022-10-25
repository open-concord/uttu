#pragma once

#include <string>

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
