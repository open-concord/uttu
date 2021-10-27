// sys api
#ifdef __APPLE__ // OSx

#endif
#ifdef __linux__ // linux and linux based
  #include <sys/socket.h> // duh
  #include <sys/types.h> // needed by socket.h
  #include <netinet/in.h> // internet socket protocol
  #include <strings.h> // bzero
  #include <unistd.h> // close, accept, etc
  #include <arpa/inet.h> // needed for ip ID
#endif
#ifdef _WIN64 // windows 64x32 systems

#endif
// standard libs
#include <vector>
#include <optional>
#include <thread>
#include <functional>
#include <memory>
#include <iostream>
#include <cstring>

struct Peer { // connected peer
private:
  // socket/ip info
  int sockfd; // socket
  struct sockaddr_in sockaddr; // socket info
  // flags
  bool local = false; // local connect? && used in logic loop
public:
  // Utility
  int Socket(); // get
  bool Local(); // get
  // Runtime
  Peer(int sock, struct sockaddr_in socka, bool local);
  void Start(void (*h)(Peer*));
  std::string Read(unsigned int t);
  void Write(std::string m, unsigned int t);
  void Close();
};

struct Session { // socket session
private:
  // socket info
  int sockfd;
  struct sockaddr_in sockaddr;
  // config
  unsigned short port;
  unsigned short queue_limit;
  // flags
  bool close = false;
  // status/connection management
  bool(*_criteria)(std::string); // accept criteria function, takes IP
public:
  // Utility
  void Criteria(bool(*criteria)(std::string));
  Session (
    unsigned short port,
    unsigned short queue_limit,
    int sockfd,
    struct sockaddr_in _self
  );
  // Runtime
  Peer Accept(); /** incoming connections */
  Peer Connect(std::string ip); /** outbound connections */
  void Close();
};

Session Create(unsigned short port, unsigned short queue_limit);

struct Timeout {
private:
  bool flag; // flag to flip on timeout
  std::thread await; // awaiting thread (to join)
  std::atomic<bool> _cancel; // cancel flag
  void _async(int ft); // inner async loop
public:
  Timeout(unsigned int t, bool* flag);
  void Cancel();
};

void errc(std::string); // configurable error handler
