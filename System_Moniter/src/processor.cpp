#include "processor.h"
#include "linux_parser.h"

using std::stof;
using std::string;

// TODO: Return the aggregate CPU utilization !
float Processor::Utilization() { 
  
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()){
  	std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >>  cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
    
  }
  float idletime = stof(idle) + stof(iowait);
  float nonidletime = stof(user) + stof(nice) + stof(system) + stof(irq) + stof(softirq) + stof(steal);
  float tot_time = idletime + nonidletime;
  
  return (nonidletime / tot_time);}

