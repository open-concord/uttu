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
  virtual struct sockaddr_in _form(int port = -1);

  /** origin */
  virtual void target(_tf t);
  virtual void port(unsigned short int _port);
  virtual void queue(int origin_fd);

  /** operations */
  virtual std::string readb();
  virtual void writeb(std::string m);
  virtual void closeb();

  /** status */
  virtual int socketfd();
  virtual std::string peer_ip();

  virtual ~np() {}
};
