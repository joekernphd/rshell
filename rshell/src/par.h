#ifndef PAR_H_
#define PAR_H_

#include "shell.h"

using namespace std;

class Parser {
   public:
     vector<string> parse(string input);
     Shell* makeCommands(string input, bool& exitVar);
     Command* constructCommand(vector<string>& args, bool& exitVar);
     
};

#endif
