#include "../inc/uttu.hpp"

Session Create(
  unsigned short port,
  unsigned short queue_limit=5
) {
  // TODO: actually check system max
  if(queue_limit > 5) {errc("QUEUE_LIMIT MUST BE LESS THAN OR EQUAL TO 5");}
  // initial setup
  struct sockaddr_in _self;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
  if(sockfd<0) {errc("COULD NOT OPEN");}
  bzero((char*) &_self, sizeof(_self)); // zeroing out self_addr buffer

  // sockaddr config(s)
  _self.sin_family = AF_INET;
  _self.sin_addr.s_addr = INADDR_ANY;
  _self.sin_port = htons(port); // made sure to convert to net byte order

  // binding socket
  if (bind(
    sockfd,
    (struct sockaddr*) &_self,
    sizeof(_self)
  ) <0) {errc("COULD NOT BIND");} // [ERROR]

  listen(sockfd, queue_limit);
  Session s(port, queue_limit, sockfd, _self);
  return s;
}
