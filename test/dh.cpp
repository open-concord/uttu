#include <uttu.hpp> /** in production <uttu> */


/** g++ -std=20 -I./../inc -L./../exe -luttu */
int main () {
  dhms d, p;

  d.Peer(p.Public());
  p.Peer(d.Public());
  d.Gen();
  p.Gen();

  std::cout << "shared:\n";
  std::cout << "(p) " << p.Shared() << std::endl;
  std::cout << "(d) " << d.Shared() << std::endl;

  std::cout << d.AD(p.AE("TEST")) << "\n";

  return 0;
};
