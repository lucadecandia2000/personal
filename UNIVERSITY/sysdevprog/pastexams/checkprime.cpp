/*
Write a small C++ example with three threads: 
 Thread take_value takes a number from the command line. 
 Thread check_if_prime checks whether the number is prime. 
 Thread give_answer prints the answer to standard output. 
Thread communication should be made using promises and futures. 
If you do not remember the exact syntax of C++ synchronization primitives, you can write 
down a mock version (with same sense...). Correctness is strictly required in the template 
syntax which is required to be right, as well as in any basic C++ syntax.
*/

#include <iostream> 
#include <thread> 
#include <vector> 
#include <future> 

using namespace std ; 


// need only a promise to set 
void * take_value(promise<int> &value){
// get from command line and pass it 
int temp ; 
cin >> temp ; 
value.set_value(temp) ; 

}

// need future to get and a promise to set 
void * check_if_prime(future<int>& val ,promise<bool> &toCheck){
    int value = val.get() ; 
    // if prime 
    toCheck.set_value(true) ; 
}

//need a future to get
void * give_answer(future<bool>& isPrime){
    bool res = isPrime.get() ; 
    cout << res << endl ; 
}


int main () {

promise<int> value ; 
thread t1(take_value, ref(value)) ;

future<int> val = value.get_future() ; 
promise<bool> toCheck ; 
thread t2(check_if_prime, ref(val), ref(toCheck)) ; 

future<bool> isPrime = toCheck.get_future() ; 
thread t3(give_answer, ref(isPrime)) ;

t1.join() ; 
t2.join() ; 
t3.join() ; 
return 1 ; 
}