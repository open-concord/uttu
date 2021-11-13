#include "../inc/uttu.hpp"

int main() {
  Session s = Create(1337, 5);
  Timeout tm(3000, s);
  s.Accept();
  
}
