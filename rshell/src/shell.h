#ifndef SHELL_H_
#define SHELL_H_
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

class Shell {
     public:
          virtual bool run() = 0;
};

class Command : public Shell {
   public:
     virtual bool run() = 0;
};

class BinCmd : public Command {
   protected:
     vector<string> tokens;
     int size;
   public:
     BinCmd(vector<string> t, int s) : tokens(t), size(s) { };
     virtual bool run();
};

class ExitCmd : public Command {
   protected:
     bool* flag;
   public:
     ExitCmd(bool& f) {
          flag = &f;

     };
     virtual bool run();
};

class TestCmd : public Command {
    protected:
      vector<string> tokens;
    public:
      TestCmd(vector<string> t) : tokens(t) { };
      virtual bool run();
};

class Connector : public Shell {
   protected:
     Shell* child1;
     Shell* child2;
   public:
     Connector(Shell* c1, Shell* c2) {
          child1 = c1;
          child2 = c2;
     };
     virtual ~Connector(){

     }
     virtual bool run() = 0;
};

class And : public Connector {
   public:
     And(Shell* c1, Shell* c2): Connector(c1, c2) { };
     virtual bool run();
};

class Or : public Connector {
   public:
     Or(Shell* c1, Shell* c2) : Connector(c1, c2) { };
     virtual bool run();
};

class Semicolon : public Connector {
   public:
     Semicolon(Shell* c1, Shell* c2) : Connector(c1, c2) { };
     virtual bool run();
};

#endif
