#include <string>

/** extendable formatting */
/** (default definition is the Concord Standard Protocol) */
struct np {
protected:
  struct sockaddr_in self;
  int cfamily = AF_INET;
  int sockfd;
public:
  struct _tf {
    std::string addr;
    unsigned short int port;
  } tf; // target format

  /** form socket */
  struct sockaddr_in _form(int port = -1);

  /** origin */
  void target(_tf t);
  void port(unsigned short int _port);
  void queue(int origin_fd);

  /** operations */
  std::string readb();
  void writeb(std::string m);
  void closeb();

  /** status */
  int socketfd();
  std::string peer_ip();

  np();
};
