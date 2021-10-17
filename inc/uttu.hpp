// sys api
#ifdef __APPLE__ // OSx

#endif
#ifdef __linux__ // linux and linux based
  #include <sys/types.h> // needed by socket.h
  #include <sys/socket.h> // duh
  #include <netinet/in.h> // internet socket protocol
#endif
#ifdef _WIN64 // windows 64&&32 systems

#endif
// standard libs
#include <vector>
#include <optional>

struct Peer { // connected peer
private:
  // socket/ip info
  int sockfd; // socket
  struct sockaddr_in sockaddr; // socket info
  // flags
  unsigned int timeout; // time between last communication to close connection
  bool host; // eg. did I initiate this communication?
  bool local; // local connect?
  bool close; // close connection?
  // buffer for read & handler for dealing with the buffer
  char* (*handler)(Peer*);
  char buffer[256];
public:
  Peer(int sock, struct sockaddr_in socka) : sockfd(sock), sockaddr(socka);
  void Set_Handler(char* (*handler)(Peer*)); // message handler, returns char* to be sent
  void Start();
  ~Peer();
};

struct Session { // socket session
private:
  // socket/ip info
  int sockfd;
  struct sockaddr_in sockaddr;
  // config
  unsigned short port;
  unsigned short queue_limit;
  // flags
  bool close = false;
  // status/connection management
  bool(*criteria)(const char*); // accept criteria function, takes IP
  std::vector<Peer> connections;
public:
  Session(
    unsigned short port,
    unsigned short queue_limit
  ) : port(port), queue_limit(queue_limit);
  Peer* Acceptor(bool(*criteria)(const char*));
  void Close();
  // TODO: user/application functions
  ~Session();
};

void error(char *emsg); // catches sys/api errors (can overwrite)

Session Create_socket(std::optional<const char*> target_ip, unsigned short port, unsigned short queue_limit=5);
// sys api dependent functions
