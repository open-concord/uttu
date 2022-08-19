#pragma once
#include <vector>

struct FlagManager {
/** flags */
protected:
  std::vector<std::vector<bool>> ftape;
public:
  void Reserve(unsigned int, unsigned int);
  void Fill(bool, unsigned int = 0);
  unsigned int FlagCount(unsigned int);
  void Set(unsigned int, bool, unsigned int = 0);
  bool Get(unsigned int, unsigned int = 0);
  FlagManager(unsigned int); // count of tapes only 
  FlagManager(std::vector<std::vector<bool>>); // literal 
  FlagManager();
};


