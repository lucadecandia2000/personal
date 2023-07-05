#include "StackClass.h"

void StackClass::push(int e) {
    stack.push_back(e); // push an elem in the back of the vector ; 
    return;
}

int StackClass::pop() {
    int e;
    e = stack.back(); // get the elem so we can return him 
    stack.pop_back(); // actually delete it 
    return (e);
}

void StackClass::visit() {
    int n;
    cout << "Stack visit: " << endl;
    n = 0;
    for (auto e : stack)  {
        cout << n++ << ". " << e << endl; // loop over the stack (vector) and get all elems ; 
    }
    return;
}

int StackClass::getSize() {
    return stack.size(); // retur the size of the stack 
}

bool StackClass::empty() {
    return (stack.size()==0); // check if size of the stack is empty ; 
}
