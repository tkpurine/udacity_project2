#include <string>

#include "format.h"
using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hh{seconds / 3600};
  long mm{(seconds % 3600) / 60};
  long ss{(seconds % 3600) % 60};

  string ret{std::to_string(hh) + ":" + std::to_string(mm) + ":" +
             std::to_string(ss)};

  return ret;
}