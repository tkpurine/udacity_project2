#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::vector;
using std::string;

class Processor {
 public:
  float Utilization();  // See src/processor.cpp
  void setvalue(vector<string> input);
  // Declare any necessary private members
 private:
  long user{0};
  long nice{0};
  long system{0};
  long idle{0};
  long iowait{0};
  long irq{0};
  long softirq{0};
  long steal{0};
  long guest{0};
  long guest_nice{0};
  long prevuser{0};
  long prevnice{0};
  long prevsystem{0};
  long previdle{0};
  long previowait{0};
  long previrq{0};
  long prevsoftirq{0};
  long prevsteal{0};
  long prevguest{0};
  long prevguest_nice{0};
};

#endif