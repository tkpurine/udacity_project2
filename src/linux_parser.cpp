#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float memtotal{0.0};
  float memfree{1.0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memtotal = stof(value);
      } else if (key == "MemFree:") {
        memfree = stof(value);
        break;
      }
    }
  }
  return (memtotal - memfree) / memtotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string value1;
  string value2;
  long uptime{0};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> value1 >> value2;
      uptime = stol(value1);
    }
  }
  return uptime;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string value;
  int idx;
  float starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      idx = 0;
      while (linestream >> value) {
        if (idx == 21) {
          starttime = stof(value);
          break;
        }
        idx += 1;
      }
    }
  }
  return (long)starttime;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> ret(10);
  string line;
  string key;
  string v1, v2, v3, v4, v5, v6, v7, v8, v9, v10;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9 >>
          v10;
      if (key == "cpu") {
        ret[0] = v1;
        ret[1] = v2;
        ret[2] = v3;
        ret[3] = v4;
        ret[4] = v5;
        ret[5] = v6;
        ret[6] = v7;
        ret[7] = v8;
        ret[8] = v9;
        ret[9] = v10;
        break;
      }
    }
  }
  return ret;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = stoi(value);
      }
    }
  }
  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        processes = stoi(value);
      }
    }
  }
  return processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmdline;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmdline);
  }
  return cmdline;
}
#include <boost/format.hpp>
// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        float ram_val = stof(value) * 0.001;
        value = (boost::format("%.2f") % ram_val).str();
        // value = std::to_string(ram_val);
        break;
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string tag;
  string id;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> id;
      if (tag == "Uid:") {
        return id;
      }
    }
  }
  return id;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string username;
  string id;
  string targetid;
  string x;
  std::ifstream filestream(kPasswordPath);
  targetid = LinuxParser::Uid(pid);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> id;
      if (id == targetid) {
        return username;
      }
    }
  }

  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  return LinuxParser::UpTime() - (long)((float)LinuxParser::ActiveJiffies(pid) /
                                        (float)sysconf(_SC_CLK_TCK));
}

float LinuxParser::Process(int pid) {
  string line;
  string value;
  int idx;
  float utime;
  float stime;
  float cutime;
  float cstime;
  float starttime;
  float cpu_usage{0.0F};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      idx = 0;
      while (linestream >> value) {
        if (idx == 13) {
          utime = stof(value);
        } else if (idx == 14) {
          stime = stof(value);
        } else if (idx == 15) {
          cutime = stof(value);
        } else if (idx == 16) {
          cstime = stof(value);
        } else if (idx == 21) {
          starttime = stof(value);
          break;
        }
        idx += 1;
      }
      float total_time = utime + stime + cutime + cstime;
      const float hertz{(float)sysconf(_SC_CLK_TCK)};
      float seconds = LinuxParser::UpTime() - (starttime / hertz);
      cpu_usage = (total_time / hertz) / seconds;
    }
  }
  return cpu_usage;
}