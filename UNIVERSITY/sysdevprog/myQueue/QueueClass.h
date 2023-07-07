#ifndef _QUEUECLASS
#define _QUEUECLASS

#include <iostream>
#include <string>
#include <list>

using std::cout;
using std::endl;
using std::string;
using std::list;

class QueueClass {
public:
    void enqueue(string);
    string dequeue();
    void visit();
    int getSize();
    bool empty();
private:
    list<string> queue; // using a  list of strings to create the queue ; 
};

#endif
