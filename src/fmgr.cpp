#include "../inc/uttu.hpp"

unsigned int FlagManager::FlagCount(unsigned int tindex) {
  return this->ftape.at(tindex).size(); 
}

void FlagManager::SetFlag( 
    unsigned int index, 
    bool state, 
    unsigned int tindex 
    ) {
  try {
    auto flags = this->ftape.at(tindex);
    if (flags.size() < index) {throw "Out of Bounds       Set";}
    flags.at(index) = state;
  } catch(const char* cem) {
    std::cout << "[!] " << cem << '\n';
  } catch(std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
  }
}

bool FlagManager::GetFlag(
    unsigned int index,
    unsigned int tindex
  ) {
  auto flags = this->ftape.at(tindex);
  try {
    return flags.at(index);
  } catch(std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
  }
  return false; /** this is misleading */
}

FlagManager::FlagManager(unsigned int tcount) {
  this->ftape.resize(tcount);
}

FlagManager::FlagManager(std::vector<std::pair<unsigned int, bool>> fttemp) {
  for (unsigned int i=0; i<fttemp.size(); i++) {
    auto fcount = std::get<0>(fttemp.at(i));
    auto state = std::get<1>(fttemp.at(i));
    this->ftape.at(i).resize(fcount);
    for (unsigned int j=0; j<fcount; j++) {
      this->ftape.at(i).at(j) = state;
    }
  } 
}

FlagManager::FlagManager(std::vector<std::vector<bool>> fttemp) {
  for (unsigned int i=0; i<fttemp.size(); i++) {
    this->ftape.at(i) = fttemp.at(i);
  }
} 
