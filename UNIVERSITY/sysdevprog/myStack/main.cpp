#include "StackClass.h"

int main() {
    StackClass myStack;
    int n, val;
// following a few examples on how to interact with the new class stack created 
    myStack.push(239);
    myStack.push(2323);
    myStack.push(5456);

    myStack.visit();
    std::cout << "Stack Size: " << myStack.getSize() << endl;

    n = 0;
    while (!myStack.empty()) {
        val = myStack.pop();
        std::cout << "Pop " << n++ << ": " << val << std::endl;
    }

    return 0;
}
