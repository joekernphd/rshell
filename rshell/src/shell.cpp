#include "shell.h"

bool BinCmd::run() {
     //char* args[size + 1];
     char* args[100];
     for(int i = 0; i < size; ++i) {
          args[i] = new char[size + 1];
          strcpy(args[i], tokens.at(i).c_str());
     }

     args[size] = NULL;

     pid_t pid = fork();
     bool ret = true;


     if(pid == 0) {
          //child
          if(execvp(args[0], args) == -1) {
               perror ("execvp");
               for(int i = 0; i < size; ++i) {
                    delete[] args[i];
               }
               exit(1);

          }

     }

     if(pid > 0) {
          //parent
          int status = 0;
          waitpid(-1, &status,  0);
          if( status == -1) {
               perror("wait");
          }

          //the bin command failed
          else if(status > 1) {
               ret = false;
          }
     }

     return ret;
}

bool ExitCmd::run() {
     *flag = false;
     return true;
}

bool TestCmd::run() {
     // for(unsigned i = 0; i < tokens.size(); ++i) {
     //      cout << tokens.at(i) << " ";
     // }
     
     if(tokens.size() == 2) {
          tokens.insert(tokens.begin() + 1, "-e");
     }
     
     char arg[100];
     strcpy(arg, tokens.at(2).c_str());
     
     struct stat buf;
     // if(stat(arg, &buf) == -1) {
     //      perror("stat");
     //      exit(0);
     // }
     stat(arg, &buf);
     
     
     if(tokens.at(1) == "-e") {
          if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode)) {
               cout << "(True)\n";
               return true;
          }
          cout << "(False)\n";
          return false;
     }
     
     else if(tokens.at(1) == "-d") {
          if(S_ISDIR(buf.st_mode)) {
               cout << "(True)\n";
               return true;
          }
          cout << "(False)\n";
          return false;
     }
     
     else if(tokens.at(1) == "-f") {
          if(S_ISREG(buf.st_mode)) {
               cout << "(True)\n";
               return true;
          }
          cout << "(False)\n";
          return false;
     }
     
     else {
          cout << "Syntax error: invalid flag\n";
          return false;
     }
     
     return true;
}

bool And::run() {
     if(child1->run()) {
          if(child2->run()) {
               return true;
          }
     }

     return false;
}

bool Or::run() {
     if(!child1->run()) {
          if(child2->run()) {
               return true;
          }

          else {
               return false;
          }
     }
     return true;

}

bool Semicolon::run() {
     child1->run();
     if(!child2->run()) {
          return false;
     }
     return true;
}
