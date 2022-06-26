#include "../inc/uttu.hpp"

unsigned int FlagManager::FlagCount() {
  return this->flags.size();
}

void FlagManager::SetFlag(unsigned int index, bool state)       {
  try {
    if (this->flags.size() < index) {throw "Out of Bounds       Set";}
    this->flags.at(index) = state;
  } catch(const char* cem) {
    std::cout << "[!] " << cem << '\n';
  } catch(std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
  }
}

bool FlagManager::GetFlag(unsigned int index) {
  try {
    return this->flags.at(index);
  } catch(std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
  }
  return false; /** this is misleading */
}

FlagManager::FlagManager(unsigned int size) {
  this->flags.resize(size);
}
FlagManager::FlagManager(unsigned int size, bool state) {
  this->flags.resize(size);
  for (const auto& f: this->flags) {
    this->flags.at(f) = state;
  }
}
FlagManager::FlagManager(std::vector<std::pair<int, bool>> initset) {
  this->flags.resize(initset.size());
  for (auto& [f, s]: initset) {
    this->SetFlag(f, s);
  }
}
  
