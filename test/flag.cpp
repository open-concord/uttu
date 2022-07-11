#include "../inc/uttu.hpp" 
#include <cassert>

int main(void) {

  FlagManager f;
  f.Reserve(0, 2);
  f.Fill(0, false);
  f.Set(1, true);
  f.Set(0, true);
  assert(f.Get(1));
  return 0;
}
