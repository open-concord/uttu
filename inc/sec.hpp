#include <cryptopp/eccrypto.h>
#include <cryptopp/secblock.h>
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>  // AutoSeededRandomPool
#include <cryptopp/oids.h> // secp256r1

#include <iostream>
#include <string>
#include <sstream>

using namespace CryptoPP;

struct dhms { // dhm suite
private:
  ECDH<ECP>::Domain ecd; // ecdhm domain
  // keys
  SecByteBlock pri; // own private
  SecByteBlock pub; // own public
  SecByteBlock ppub; // peer public
  SecByteBlock shared; // both shared
  // utility
  void Set(std::string k, SecByteBlock* t); // used to set keys
  std::string Get(SecByteBlock* k); // used to get keys
public:
  // generators
  dhms();
  void Keys(AutoSeededRandomPool* rp); // generate key pair
  void Gen(); // this may take a bit if ur a potato
  // setters/getters
  void Peer(std::string p); // set ppub
  std::string Public(); // pull pub key
  std::string Shared(); // get shared secret
};
