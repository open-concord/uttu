#include "../inc/uttu.hpp" /** in production <uttu> */

bool watchdog(std::string ip) {
  std::cout << "IP: " << ip << "\n";
  return true; // Anybody can be a peer!
}

void hh(Peer* p) {
  std::cout << "new msg: " << p->Read(3000) << "\n";
  std::cout << "Is local? " << p->Local() << "\n";
  p->Write("i use arch btw\n" /** message */, 3000 /** timeout */);
  p->Close();
}

void ch(Peer* p) {
  p->Write("i use arch btw\n" /** message */, 3000 /** timeout */);
  std::cout << "new msg: " << p->Read(3000) << "\n";
  std::cout << "Is local? " << p->Local() << "\n";
  p->Close();
}

void event(std::shared_ptr<Peer> p) {
  std::cout << "event called\n";
  p->Start(&hh);
}

int main () {
  Session h = Create(1337, 5);
  h.Criteria(&watchdog);
  h.Open();
  std::cout << h.Socket() << "\n";

  Session c = Create(1338, 5);
  c.Criteria(&watchdog);

  /** await connections */
  h.Lazy(&event, false);
  std::shared_ptr<Peer> p = c.Connect("127.0.0.1:1337");
  std::cout << "Connected\n";
  p->Start(&ch);
  return 0;
};
