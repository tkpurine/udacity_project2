#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return this->pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return this->cpuutilization_; }

// Return the command that generated this process
string Process::Command() { return this->command_; }

// Return this process's memory utilization
string Process::Ram() { return this->ram_; }

// Return the user (name) that generated this process
string Process::User() { return this->user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return this->uptime_; }

void Process::Update(std::string command, std::string user, std::string ram,
                     float cpuutilization, long int uptime) {
  this->command_ = command;
  this->user_ = user;
  this->ram_ = ram;
  this->cpuutilization_ = cpuutilization;
  this->uptime_ = uptime;
}

// Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpuutilization_ > a.cpuutilization_;
}