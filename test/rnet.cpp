#include "../inc/uttu.hpp"

void filler_protocol(Peer* p) {
  std::cout << "got new peer\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  std::cout << "rr: " << p->Raw_Read(3000) << '\n';
  std::cout << "closing... p\n";
  p->Close();
}


int main(void) {
  /** open first peer as a relay */
  np npA;
  Relay r(npA, 1337, 3000, 5);
  r.Open();
  r.Load(&filler_protocol);

  r.Lazy(false);

  /** connect to relay */
  np npB;
  Peer p0(npB, 1338, 3000);
  p0.Connect("127.0.0.1:1337");

  p0.Raw_Write("hello!", 3000);

  r.Close();
  p0.Close();
  return 0;
}
