#include "QueueClass.h"

void QueueClass::enqueue(string e) {
    queue.push_front(e);              // add an element in the front ; 
    return;
}

string QueueClass::dequeue() {
    string e;
    e = queue.back();  // getting the elem for visualization as the pop_back does not return it 
    queue.pop_back(); // actually remove the elem at the back 
    return (e);
}

void QueueClass::visit() {
    int n;
    cout << "Queue visit: " << endl;
    n = 0;
    for (auto e : queue)  {
        cout << n++ << ". " << e << endl; // iterate over the queue 
    }
    return;
}

int QueueClass::getSize() {
    return queue.size(); // get the size 
}

bool QueueClass::empty() {
    return (queue.size()==0);  // check if empty and return bool ; 
}
