#include "../inc/uttu.hpp"
#include "../proto/csp/inc/csp.hpp"
#include <optional>

void filler(Peer* p) {
  std::cout << "got new peer\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  std::cout << "sockfd: " << p->net->socketfd() << '\n';
  std::cout << "rr: " << p->Raw_Read(3000) << '\n';
  std::cout << "closing... p\n";
  p->Close(); 
}


int main(void) {
  /** open first peer as a relay */
  //Relay r(std::nullopt, 1337, 3000, 5); 
  //r.Swap(&filler);
  //r.Lazy(false);
  //r.Open();

  /** connect to relay */
  Peer p(std::nullopt, (unsigned short) 1338);
  p.Connect("127.0.0.1:1337");
  p.Raw_Write("hello!", 3000);
  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Too fast for its own good :)
  p.Close();
  std::cout << "all done!\n";
  return 0;
}
