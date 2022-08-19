#include "../inc/dhm.hpp"

#include <iostream>

using namespace CryptoPP;

dhms::dhms() : ecd(ECDH<ECP>::Domain(ASN1::secp256r1()/** NIST compliant 256 bit curve */)),
    pri(NULL, this->ecd.PrivateKeyLength()),
    pub(NULL, this->ecd.PublicKeyLength()),
    ppub(NULL, this->ecd.PublicKeyLength()),
    shared(NULL, this->ecd.AgreedValueLength())
{
  this->Keys(); // gen new keys
}

SecByteBlock dhms::_Set(std::string k) {
  std::string rk;
  /** written so it's blocking */
  HexDecoder ex(new StringSink(rk));
  ex.Put((const byte*)k.data(), k.size(), true /** blocking */);
  ex.MessageEnd();
  SecByteBlock sb(reinterpret_cast<const byte*>(&rk[0]), rk.size());
  return sb;
}

std::string dhms::_Get(SecByteBlock* k) {
  std::string rk, ek;
  SecByteBlock bk = *k;
  rk = std::string(reinterpret_cast<const char*>(&bk[0]), bk.size());
  /** written so it's blocking */
  HexEncoder dx(new StringSink(ek));
  dx.Put((const byte*)rk.data(), rk.size(), true /** blocking */);
  dx.MessageEnd();
  return ek;
}

void dhms::Keys() {
  AutoSeededRandomPool rp;
  ecd.GenerateKeyPair(rp, this->pri, this->pub);
}

void dhms::Gen() {
  if (!this->ecd.Agree(this->shared, this->pri, this->ppub)) {
    // ERROR
    std::cout << "[!] Could not agree on shared secret\n";
  }
}

bool dhms::Zero(std::string k) {
  for (unsigned int i=0; i < k.length()-1; i++) {
    if (k[i] != '0') {return false;}
  }
  return true;
}

void dhms::Peer(std::string p) {
  this->ppub = this->_Set(p);
}

std::string dhms::Public() {
  return this->_Get(&this->pub);
}

std::string dhms::Shared() {
  return this->_Get(&this->shared);
}

std::string dhms::AE(std::string in) {
  GCM<AES>::Encryption e;
  AutoSeededRandomPool rng;

  /** generate iv*/
  SecByteBlock iv(e.IVSize());
  rng.GenerateBlock(iv, iv.size());

  /** setting shared & iv to e */
  std::string siv = this->_Get(&iv);
  e.SetKeyWithIV(this->shared, this->shared.size(), iv, iv.size());

  /** pump */
  std::string out; // enc out
  StringSource(in, true,
    new AuthenticatedEncryptionFilter(
      e,
      new HexEncoder(
        new StringSink(out)
      ),
      false,
      this->tag
    )
  );
  /** appened iv to out */
  return out+"$"+siv;
}

std::string dhms::AD(std::string in) {
  /** seperate msg */
  std::string sin = in.substr(0, in.find("$")); // 0->delimter
  std::string siv = in.substr((in.find("$")+1)); // delimter->end

  /** GCM engine */
  GCM<AES>::Decryption d;

  /** pull iv */
  SecByteBlock iv = this->_Set(siv);
  d.SetKeyWithIV(this->shared, this->shared.size(), iv, iv.size());

  /** AES */
  std::string out;

  AuthenticatedDecryptionFilter df (
    d,
    new StringSink(out),
    AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
    this->tag
  );

  StringSource(sin, true,
    new HexDecoder(
      new Redirector(df)
    )
  );

  if(df.GetLastResult() == true) {
    return out;
  }
}
