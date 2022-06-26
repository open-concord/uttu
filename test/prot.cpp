/** Protocol Check */
#include "../inc/uttu.hpp"
#include "../proto/csp/inc/csp.hpp"

int main(void) {
  /** create a csp obj */
  csp cpt;
  /** pass to peer */
  Peer pr(&cpt);
  /** call peer owned proto method */
  pr.net->socketfd();
  
  /** same as before, but with assumed csp */
  Peer apr;
  apr.net->socketfd();
  return 0;
}
