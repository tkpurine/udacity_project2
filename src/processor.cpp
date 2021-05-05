#include "processor.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long previdle{this->previdle + this->previowait};
  long idle{this->idle + this->iowait};
  long prevnonidle{this->prevuser + this->prevnice + this->prevsystem +
                   this->previrq + this->prevsoftirq + this->prevsteal};
  long nonidle{this->user + this->nice + this->system + this->irq +
               this->softirq + this->steal};
  long prevtotal{previdle + prevnonidle};
  long total{idle + nonidle};
  long totald{total - prevtotal};
  long idled{idle - previdle};

  return (float)(totald - idled) / (float)totald;
}
void Processor::setvalue(vector<string> cpu_proc_info) {
  this->prevuser = this->user;
  this->prevnice = this->nice;
  this->prevsystem = this->system;
  this->previdle = this->idle;
  this->previowait = this->iowait;
  this->previrq = this->irq;
  this->prevsoftirq = this->softirq;
  this->prevsteal = this->steal;
  this->prevguest = this->guest;
  this->prevguest_nice = this->guest_nice;
  this->user = stol(cpu_proc_info[0]);
  this->nice = stol(cpu_proc_info[1]);
  this->system = stol(cpu_proc_info[2]);
  this->idle = stol(cpu_proc_info[3]);
  this->iowait = stol(cpu_proc_info[4]);
  this->irq = stol(cpu_proc_info[5]);
  this->softirq = stol(cpu_proc_info[6]);
  this->steal = stol(cpu_proc_info[7]);
  this->guest = stol(cpu_proc_info[8]);
  this->guest_nice = stol(cpu_proc_info[9]);
}