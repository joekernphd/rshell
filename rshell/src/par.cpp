#include "par.h"

vector<string> Parser::parse(string input){
     string::iterator it = input.begin(); // starts iterator at start of string
     string toBePushed, comparator; // two temps
     vector<string>tokens; // tokenvector

     while(it != input.end()) { // while the iterator doesnt hit the end of the string

          comparator = *it; // comparator is used for comparisons
          if(comparator == " " || comparator == ";") { // if it iterates thru and finds a whitespace or ;
               if(toBePushed.empty() == false) {
                    tokens.push_back(toBePushed);
                    toBePushed.clear();
               }

               if(comparator == ";") { // pushes toBePushed and the semicolon to the tokenvector
                         tokens.push_back(comparator); 
               }

               ++it; // moves to next iteration
          }

          else { // if no whitespace or ;

               if(comparator == "\"") { // if it finds a ", go until you find the next "
                    bool condition = true;
                    string comparator2;
                    it++;

                    while(condition) {
                         comparator2 = *it;

                         // after it finds the second ", push toBePushed, clear it and move onto the next character AFTER the second "
                         if(comparator2 == "\"") {
                              condition = false;
                              tokens.push_back(toBePushed);
                              toBePushed.clear();
                              ++it;
                         }

                         // if couldn't find ", moves to next iteration
                         else {
                              toBePushed += *it;
                              ++it;
                         }
                    }
               }

               else if(comparator == "\'") { // if it finds a ', go until you find the next '
                    bool condition = true;
                    string comparator2;
                    it++;

                    while(condition) {
                         comparator2 = *it;

                         // after it finds the second ', push toBePushed, clear it and move onto the next character AFTER the second '
                         if(comparator2 == "\'") {
                              condition = false;
                              tokens.push_back(toBePushed);
                              toBePushed.clear();
                              ++it;
                         }

                         // if couldn't find ', moves to next iteration
                         else {
                              toBePushed += *it;
                              ++it;
                         }
                    }
               }

               else if(comparator == "#") { // everything after the comment is a single token
                    if(!toBePushed.empty()) {
                         tokens.push_back(toBePushed);
                         toBePushed.clear();
                    }
                    
                    toBePushed = *it;
                    tokens.push_back(toBePushed);
                    toBePushed.clear();

                    while(it != input.end()) {
                         ++it;
                         toBePushed += *it;
                    }

                    tokens.push_back(toBePushed);
                    toBePushed.clear();
               }
               
               else if(comparator == "[") {
                    if(!tokens.empty()) {
                         if(tokens.back() == ";" || tokens.back() == "||" || tokens.back() == "&&" ) {
                              bool condition = true;
                              string comparator2;
                              ++it;
                              
                              tokens.push_back("[");
                              
                              while(condition) {
                                   comparator2 = *it;
                                   if(comparator2 == "]") {
                                        tokens.push_back(toBePushed);
                                        toBePushed.clear();
                                        condition = false;
                                        ++it;
                                   }
                                   
                                   else {
                                        toBePushed += *it;
                                        ++it;
                                   }
                              }
                         }
                         
                         else {
                              toBePushed += *it;
                              ++it;
                         }
                         
                    }
                    
                    else if(tokens.empty()) {
                         bool condition = true;
                         string comparator2;
                         ++it;
                         
                         tokens.push_back("[");
                         
                         while(condition) {
                              comparator2 = *it;
                              if(comparator2 == "]") {
                                   tokens.push_back(toBePushed);
                                   toBePushed.clear();
                                   condition = false;
                                   ++it;
                              }
                              
                              else {
                                   toBePushed += *it;
                                   ++it;
                              }
                         }
                    }
                    
                    else {
                        toBePushed += *it;
                        ++it; 
                    }
               }

               else if (comparator == "(") {
                    bool condition = true;
                    string comparator2;
                    it++;
                    
                    tokens.push_back("(");
                    int count = 0; //for handling nested parentheses
                    while(condition) {
                         comparator2 = *it;

                         // after it finds the second ), push toBePushed, clear it and move onto the next character AFTER the second "
                         if(comparator2 == "(") {
                              ++count;  //found a nested parentheses
                         }
                         if(comparator2 == ")" && count == 0) {
                              condition = false;
                              tokens.push_back(toBePushed);
                              // tokens.push_back(")");
                              toBePushed.clear();
                              ++it;
                         }
                         else if(comparator2 == ")" && count > 0) {
                              --count;  //one level of nested done
                              toBePushed += *it;
                              ++it;
                         }
                         // if couldn't find ), moves to next iteration
                         else {
                              toBePushed += *it;
                              ++it;
                         }
                    }
               }

               // if neither ", or ' was found, it means its still a command/arg, so iterate thru and add to toBePushed
               else {
                    toBePushed += *it;
                    ++it;
               }
          }
     }

     // deals with the ending case
     if(toBePushed.empty() == false) {
          tokens.push_back(toBePushed);
     }

     // for(unsigned i = 0; i < tokens.size(); ++i) {
     //      cout << "TOKEN #" << i << " " << tokens.at(i) << endl;
     // }

     return tokens;
}

Command* Parser::constructCommand(vector<string>& args, bool& exitVar) {
     if(args.size() != 0) {
          if(args.at(0) == "exit") {
               return new ExitCmd(exitVar);
          }
          else if(args.at(0) == "test") {
               return new TestCmd(args); 
          }
          else {
               return new BinCmd(args, args.size());
          }
     }
     return NULL;
}


Shell* Parser::makeCommands(string input, bool& exitVar) {
     vector<string> tokens = parse(input);
     vector<string> connectors;
     vector<Shell*> commands;
     vector<string> toConstruct;
     // cout << tokens.size() << endl;
     for(unsigned i = 0; i < tokens.size(); ++i) {
          // cout << "Token: #" << i << " \'" << tokens.at(i) << '\'' << endl;
          //if its a connector then construct a command with said arguments
          if(tokens.at(i) == "(") {
               ++i;
               string temp = tokens.at(i);
               commands.push_back(makeCommands(temp, exitVar));
               if(commands.back() == NULL) {
                    commands.pop_back();
               }
          }
          
          else if(tokens.at(i) == "[") {
               ++i;
               string temp = tokens.at(i);
               vector<string> passMe = parse(temp);
               passMe.insert(passMe.begin(), "test");
               commands.push_back(constructCommand(passMe, exitVar));
          }
          
          else if(tokens.at(i) == ";" || tokens.at(i) == "||" || tokens.at(i) == "&&"){
               //handle trailing connectors
               if(i != tokens.size() - 1) {
                    connectors.push_back(tokens.at(i));
                    if(!toConstruct.empty()) {
                         commands.push_back(constructCommand(toConstruct, exitVar));
                         toConstruct.clear();
                    }
               }
               else {
                    if(!toConstruct.empty()) {
                         commands.push_back(constructCommand(toConstruct, exitVar));
                    }
               }

          }
          
          
          else if(tokens.at(i) == "#") {
               if(toConstruct.size() != 0) {
                   commands.push_back(constructCommand(toConstruct, exitVar));
               }

               i = tokens.size();
          }

          // //if it is not a connector it is a command or an argument
          else {
               toConstruct.push_back(tokens.at(i));
               if(i == tokens.size() - 1 && !toConstruct.empty()) {
                    commands.push_back(constructCommand(toConstruct, exitVar));
               }
          }
     }
     
     // cout << "connectors: " << connectors.size() << endl;
     vector<Connector*> constructedConnectors;
     
     for(unsigned i = 0; i < connectors.size(); ++i) {
          if(i == 0) {
               if(commands.size() < 2) {
                    cout << "Syntax error - leading connector\n";
                    exit(0);
               }
               if(connectors.at(i) == ";") {
                    constructedConnectors.push_back(new Semicolon(commands.at(0), commands.at(1)));
               }
               else if(connectors.at(i) == "||") {
                    constructedConnectors.push_back(new Or(commands.at(0), commands.at(1)));
               }
               else {
                    constructedConnectors.push_back(new And(commands.at(0), commands.at(1)));
               }

          }
          else {
               if(connectors.at(i) == ";") {
                    constructedConnectors.push_back(new Semicolon(constructedConnectors.at(i - 1), commands.at(i + 1)));
               }
               else if(connectors.at(i) == "||") {
                    constructedConnectors.push_back(new Or(constructedConnectors.at(i - 1), commands.at(i + 1)));
               }
               else {
                    constructedConnectors.push_back(new And(constructedConnectors.at(i - 1), commands.at(i + 1)));
               }
          }
     }
     
     //connectors constructed, run the last one and let recursion work
     if(!constructedConnectors.empty()) {
          return constructedConnectors.at(constructedConnectors.size() - 1);
          
     }
     
     //single command
     else if(!commands.empty()) {
          return commands.at(0);
     }
     
     else {
          return NULL;
     }
}
