#include <string>
#include <stdlib.h>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  	char tring[10];
    int H = seconds / 3600;
    int r = seconds % 3600;
    int M = r / 60;
    int S = r % 60;
    sprintf(tring, "%.2d:%.2d:%.2d", H, M, S);
    return tring;
   }
