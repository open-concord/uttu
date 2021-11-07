#include "../inc/uttu.hpp" /** in production <uttu> */

bool watchdog(std::string ip) {
  std::cout << "IP: " << ip << "\n";
  return true; // Anybody can be a peer!
}

void handler(Peer* p) {
  std::cout << "new msg: " << p->Read(3000) << "\n";
  std::cout << "Is local? " << p->Local() << "\n";
  p->Write("i use arch btw\n" /** message */, 3000 /** timeout */);
  p->Close();
}

int main () {
  Session st = Create(1338, 5);
  st.Criteria(&watchdog);

  std::cout << "made session\n";

  std::shared_ptr<Peer> p = st.Connect("127.0.0.1", 1337);
  std::cout << "connect done\n";
  p->Start(&handler);

  return 0;
};
