/** sys api */
#ifdef __APPLE__ // OSx

#endif
#ifdef __linux__ // linux and linux based
  #include <sys/socket.h> // duh
  #include <sys/types.h> // needed by socket.h
  #include <netinet/in.h> // internet socket protocol
  #include <strings.h> // bzero
  #include <unistd.h> // close, accept, etc
  #include <arpa/inet.h> // needed for ip ID
  #include <netdb.h> // gethostbyname
  #include <poll.h> // ppoll & poll
#endif
#ifdef _WIN64 // windows 64x32 systems
  #include <ws2def.h>
#endif

/** standard libs */
#include <vector>
#include <optional>
#include <thread>
#include <functional>
#include <memory>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>

/** outside */
#include <nlohmann/json.hpp>
#include <cryptopp/eccrypto.h>
#include <cryptopp/secblock.h>
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>  // AutoSeededRandomPool
#include <cryptopp/oids.h> // secp256r1
#include <cryptopp/rijndael.h> // AES
#include <cryptopp/gcm.h>  // GCM
#include <cryptopp/hex.h> // HexEncoder/Decoder

using json = nlohmann::json;
using namespace CryptoPP;

struct dhms { // dhm suite
private:
  const int tag = 12; // GCM tag size
  ECDH<ECP>::Domain ecd; // ecdhm domain
  // keys
  SecByteBlock pri; // own private
  SecByteBlock pub; // own public
  SecByteBlock ppub; // peer public
  SecByteBlock shared; // both shared
  // key utility (interfaced via hex)
  SecByteBlock _Set(std::string k); // used to set keys
  std::string _Get(SecByteBlock* k); // used to get keys
public:
  // generators
  dhms();
  void Keys(); // generate key pair
  void Gen(); // this may take a bit if ur a potato
  // setters/getters
  void Peer(std::string p); // set ppub
  std::string Public(); // pull pub key
  std::string Shared(); // get shared secret
  std::string AE(std::string in); // Authenticated Encryption
  std::string AD(std::string in); // Authenticated Decryption
};

struct Peer { // connected peer
private:
  // socket/ip info
  dhms sec; // dhms
  int sockfd; // socket
  struct sockaddr_in sockaddr; // socket info
  // flags
  bool local = false; // local connect? && used in logic loop
  bool host = true;
  unsigned int tout;
  // raw functions
  std::string Raw_Read(unsigned int t);
  void Raw_Write(std::string m, unsigned int t);
public:
  // Utility
  int Socket(); // get
  bool Local(); // get
  bool Host(); // get
  // Runtime
  Peer(int sock, struct sockaddr_in socka, bool local, bool host, unsigned int timeout);
  void Key_Exchange();
  std::string Read(unsigned int t);
  void Write(std::string m, unsigned int t);
  void Close();
};

struct Session { // socket session
private:
  // config
  unsigned short port;
  unsigned short queue_limit;
  // socket info
  int sockfd;
  struct sockaddr_in sockaddr;
  // flags
  bool close = false;
  unsigned int tout;
  // status/connection management
  std::function<bool(std::string)> _criteria; // accept criteria function, takes IP
  // inner thread loop for Lazy()
  void _Lazy(std::function<void(std::shared_ptr<Peer>)> h);
public:
  // Utility
  int Socket(); // get
  void Criteria(std::function<bool(std::string)> criteria); // set
  Session (
    unsigned short port,
    unsigned short queue_limit,
    int sockfd,
    struct sockaddr_in _self,
    unsigned int timeout
  );
  // Runtime
  void Open();
  std::shared_ptr<Peer> Accept(); /** incoming connections */
  std::shared_ptr<Peer> Connect(std::string target/**(ip:port) */); /** outbound connections */
  void Lazy(std::function<void(std::shared_ptr<Peer>)> h, bool blocking); /** lazy accept */
  void Close();
};

struct Timeout {
private:
  int sk; // socket to kill on timeout
  std::thread expire;
  std::atomic<bool> _cancel; // cancel flag
  void _async(int ft); // inner async loop
public:
  Timeout(unsigned int t, int s);
  void Cancel();
};

Session Create(
  unsigned short port,
  unsigned short queue_limit,
  unsigned int timeout
);

void errc(std::string); // configurable error handler
void Kill_Socket(int s); // called on timeout; configurable
