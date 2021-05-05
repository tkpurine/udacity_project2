#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, std::string cmdline, std::string user, std::string ram,
          float cpuutilization, long uptime)
      : pid_(pid),
        command_(cmdline),
        user_(user),
        ram_(ram),
        cpuutilization_(cpuutilization),
        uptime_(uptime) {}
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  void Update(std::string command, std::string user, std::string ram,
              float cpuutilization, long int uptime);
  bool operator<(Process const& a) const;

  // Declare any necessary private members
 private:
  int pid_{0};
  std::string command_{};
  std::string user_{};
  std::string ram_;
  float cpuutilization_;
  long int uptime_;
};

#endif