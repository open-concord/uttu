#include "../inc/sec.hpp"

using namespace CryptoPP;

dhms::dhms() : ecd(ECDH<ECP>::Domain(ASN1::secp256r1()/** NIST compliant 256 bit curve */)),
    pri(this->ecd.PrivateKeyLength()),
    pub(this->ecd.PublicKeyLength()),
    ppub(this->ecd.PublicKeyLength()),
    shared(this->ecd.AgreedValueLength())
{
  AutoSeededRandomPool rp;
  this->Keys(&rp); // gen new keys
}

std::string dhms::Get(SecByteBlock* k) {
  Integer s;
  s.Decode(k->BytePtr(), k->SizeInBytes());

  std::stringstream ss;
  ss << std::hex << s;
  return ss.str();
}

void dhms::Set(std::string k, SecByteBlock* t) {
  Integer ik(k.c_str()); // str to cryptopp::Int
  size_t es = ik.MinEncodedSize(Integer::UNSIGNED);
  t->resize(es);
  ik.Encode(t->BytePtr(), es, Integer::UNSIGNED); // push to sec byte block
}

void dhms::Keys(AutoSeededRandomPool* rp) {
  ecd.GenerateKeyPair(*rp, this->pri, this->pub);
}

void dhms::Gen() {
  if (!this->ecd.Agree(this->shared, this->pri, this->ppub)) {
    // ERROR
    std::cout << "Could not agree on shared secret\n";
  }
}

void dhms::Peer(std::string p) {
  this->Set(p, &ppub);
}

std::string dhms::Public() {
  return this->Get(&this->pub);
}

std::string dhms::Shared() {
  return this->Get(&this->shared);
}
