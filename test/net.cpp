#include "../inc/uttu.hpp"
#include "../proto/csp/inc/csp.hpp"

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
  csp t1;
  Relay r(&t1, 1337, 3000, 5); 
  r.Swap(&filler);
  r.Lazy(false);
  r.Open();

  /** connect to relay */
  csp t2;
  Peer p(&t2, (unsigned short) 1338);
  p.Connect("127.0.0.1:1337");
  p.Raw_Write("hello!", 3000);
  
  /** give enough time for both parties to close */
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  return 0;
}
