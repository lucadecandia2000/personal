#include "QueueClass.h"

int main() {
    QueueClass myQueue;
    int n;
    string val;
// following a list of example on how to use this class 
    myQueue.enqueue("luca");
    myQueue.enqueue("de");
    myQueue.enqueue("candia");
    myQueue.enqueue("copy");

    myQueue.visit();
    std::cout << "Size is :  " << myQueue.getSize() << endl;

    n = 0;
    while (!myQueue.empty()) {
        val = myQueue.dequeue();
        std::cout << "Dequeue " << n++ << ": " << val << std::endl;
    }

    return 0;
}
