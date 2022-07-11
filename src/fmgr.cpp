#include "../inc/uttu.hpp"

void FlagManager::Reserve(unsigned int tape, unsigned int count) {
  if (this->ftape.size() <= tape) {
    this->ftape.resize(tape+1);
  }
  this->ftape.at(tape).resize(count);
}

void FlagManager::Fill(bool state, unsigned int tindex) {
  try {
    for (unsigned int i=0; i<this->ftape.at(tindex).size(); i++) {
      this->ftape.at(tindex).at(i) = state;
    }
  } catch (std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
  }
}

unsigned int FlagManager::FlagCount(unsigned int tindex) {
  return this->ftape.at(tindex).size(); 
}

void FlagManager::Set( 
    unsigned int index, 
    bool state, 
    unsigned int tindex 
    ) {
  try {
    if (this->ftape.size() <= tindex) {
      std::cout << "[%] Starting new tape @ index " << tindex << '\n';
      this->ftape.resize(this->ftape.size()+1);
      this->ftape.at(tindex).resize(index);
    }
    if (this->ftape.at(tindex).size() <= index) {
      std::cout << "[Warn] Out of Bounds Set\n";
      this->ftape.at(tindex).resize(index+1);
    }
    this->ftape.at(tindex).at(index) = state;
  } catch(std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
 }
}

bool FlagManager::Get(
    unsigned int index,
    unsigned int tindex
  ) {
  try {
    return this->ftape.at(tindex).at(index);
  } catch(std::exception& e) {
    std::cout << "[!!] " << e.what() << '\n';
  } 
}

FlagManager::FlagManager(unsigned int tcount) {
  this->ftape.resize(tcount);
}

FlagManager::FlagManager(std::vector<std::vector<bool>> temp) {
  this->ftape.resize(temp.size());
  for (unsigned int i=0; i<temp.size(); i++) {
    this->ftape.at(i) = temp.at(i);
  }
}

FlagManager::FlagManager() {}
