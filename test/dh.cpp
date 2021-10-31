#include "../inc/uttu.hpp" /** in production <uttu> */

int main () {
  dhms d("", "");
  dhms p("", "");

  d.Peer(p.Public());
  d.Gen();

  p.Peer(d.Public());
  p.Gen();

  std::cout << "shared:\n";
  std::cout << "(p) " << p.Shared() << std::endl;
  std::cout << "(d) " << d.Shared() << std::endl;

  return 0;
};
