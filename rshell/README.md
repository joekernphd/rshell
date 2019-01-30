# rshell
Our design is a shell built with OOP in C++. The basic design consists of a Shell base class with a composite class Connector.
Connector contains two Shell pointers, child1 and child2. Our abstract leaf class is Command which inherits from Shell and has command and arguements.
Our program correctly outputs the username and hostname, as a shell should. After extensive testing, no bugs have been
found that have not been fixed.

hw3 update:
we added parentheses functionality and we also added the test command, both long hand and short hand using [].
We did not run into any issues because of the structure of our program, not bugs have been found that were not promptly fixed.

