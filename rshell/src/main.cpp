#include "par.h"

int main() {

     Parser* par = new Parser();
     string line;
     bool isRunning = true;

     while(isRunning) {
          char hostname[100];
          char username[100];
          gethostname(hostname, 100);
          getlogin_r(username, 100);
          cout << username << "@" << hostname << "$ ";
          getline(cin, line, '\n'); // grabs line
          //makeCommands(par, line)->run();
          if(par->makeCommands(line, isRunning) != NULL) {
               par->makeCommands(line, isRunning)->run();
          }
     }


     return 0;
}
