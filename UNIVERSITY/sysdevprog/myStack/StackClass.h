#ifndef _STACKCLASS
#define _STACKCLASS

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

class StackClass {
public:
    void push(int); // push an elem in the stack 
    int pop(); // pop the first elem 
    void visit(); // get the list of elem in the stack 
    int getSize(); // get the size of the stack 
    bool empty(); // check if the stack is empty 
private:
    vector<int> stack; // the stack is implemented using the vector container, other possibilities are of course possible ;
};
 // the stack itself is set to be private but all the methods are accessible ;
#endif
