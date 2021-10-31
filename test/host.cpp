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
  int i = 3;
  Session st = Create(1337, 5);
  st.Criteria(&watchdog);

  /** await connections */
  for (int j=0; j<i; j++) {
    Peer new_peer = st.Accept();
    new_peer.Start(&handler);
  };

  return 0;
};
