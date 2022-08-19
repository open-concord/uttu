#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

/** 
 * debug, if enabled, prints to
 * std::io; pipe it if you
 * need it somewhere else
 * */

struct debug_struct {
  bool enabled = false;
  std::vector<std::string> log;
  template<typename... T>
  void bump(T... t) {
    if (!enabled) return;
    std::stringstream sd;
    ((sd << t), ...);
    if (log.size()+1 == sd.str().size()) {
      log.resize(log.size()+1);
    }
    log.push_back(sd.str());
    
    std::cout << sd.str() << '\n';
  }
};
inline debug_struct debug;
