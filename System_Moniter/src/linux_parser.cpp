#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stoi;
using std::stol;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string mem_key, mem_tot, mem_free;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> mem_key >> mem_tot;
    std::getline(stream, line);
    linestream.str(line);
    linestream.clear();
    linestream >> mem_key >> mem_free ;
    }
  float mem_use = stof(mem_tot)- stof(mem_free);
  float pmem_use = mem_use/stof(mem_tot);
  return pmem_use;
  }


// TODO: Read and return the system uptime ! At include one class 

long LinuxParser::UpTime() {
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return stol(uptime);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(int pid) {
	string line, cat, utime, stime, cutime, cstime;
  	std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  	if (stream.is_open()){
      while (std::getline(stream, line)){
      	std::istringstream linestream(line);
      	for (int i = 1; i < 14; i++){
        	linestream >> cat;
        }
      	linestream >> utime >> stime >> cutime >> cstime;
      	long totaltime = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
      	return totaltime;
    }
   }
  return -1;
}

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
//vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes !
int LinuxParser::TotalProcesses() { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
  	while(std::getline(stream, line)){
    	std::istringstream linestream(line);
        linestream >> key >> value;
      	if (key == "processes"){
        	return stoi(value);
        }
    }
  }
  return 1;
}

// TODO: Read and return the number of running processes  !
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
  	while(std::getline(stream, line)){
    	std::istringstream linestream(line);
        linestream >> key >> value;
      	if (key == "procs_running"){
        	return stoi(value);
        }
    }
  }
  return 1;
}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line; 
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
  	std::getline(stream, line);
    return line;
  }
  return "-1";
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
	string key, value;
  	string line;
  	std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  	if (stream.is_open()){
    	while(std::getline(stream, line)){
        	std::istringstream linestream(line);
          	linestream >> key >> value;
          	if (key == "VmSize:"){
            	return to_string(stoi(value) / 1024); 
            }
        }
    }
  return "-1";
}


// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
	string key, value;
  	string line;
  	std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  	if (stream.is_open()){
    	while(std::getline(stream, line)){
        	std::istringstream linestream(line);
          	linestream >> key >> value;
          	if (key == "Uid:"){
            	return value;
            }
        }
    }
  return "-1";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function

string LinuxParser::User(int pid) {
  string line;
  string user, x, uid_l;
  string uid_r{LinuxParser::Uid(pid)};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid_l) {
        if (uid_l == uid_r) {
          return user;
        }
      }
    }
  }
  return "-1";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function


long LinuxParser::UpTime(int pid) {
  string line;
  string var;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i < 23; i++) {
        linestream >> var; // Grab the 22nd value
      }
      return stol(var) / sysconf(_SC_CLK_TCK); // Return uptime in seconds
    }
  }
return -1;
}