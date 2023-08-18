/*
Write a small C++ program where: 
    - A shared integer variable var is initialized at the beginning to 0; 
    - A thread “plus3” adds 3 to var every 3 seconds; 
    - A thread “minus1” subtracts 1 to var every 2 seconds; 
    - A thread “check10” checks var every 10 seconds and: 
    - If var is less than 10, it does nothing; 
    - If var is greater or equal to 10, it terminates the whole program. 
The function to put a thread in the sleep status (e.g., for 1 second) is the following one: 
std::this_thread::sleep_for (std::chrono::seconds(1)) 
Write the code of the program and manage threads synchronization. 
Make sure all threads finish running before the main program terminates. 
*/
using namespace std ; 
mutex m ; 
bool finished = false ;
int var = 0 ; 
void plus3() {
  while(true) {
    unique_lock<mutex> check_lock{m} ;  //all operations on the var must be done after acquiring the lock ; 
    if(var > 10) {
        finished = true ; 
        check_lock.unlock() ; // it is a infinite loop, so we need to release the lock manually cause it never goes out of scope ;
        return ; 
    } else {
       cout << "var is < 10 " <<endl ;  
    }
    check_lock.unlock() ; 
    this_thread::sleep_for (std::chrono::seconds(10));
  }
}
void minus1() {
    while(true) { 
        unique_lock<std::mutex> minus_lock{m}; 
        if(!finished){ 
        var-=1; 
        std::cout << "Minus1 work done. Var = " << var << std::endl; 
        minus_lock.unlock(); 
        } else{                      // it is a infinite loop, so we need to release the lock manually cause it never goes out of scope ; 
        minus_lock.unlock(); 
        return; 
        } 
        this_thread::sleep_for (std::chrono::seconds(2)); 
    } 
}
void check10() {
    while(true) { 
        unique_lock<std::mutex> plus_lock{m}; 
        if(!finished){ 
        var+=3; 
        cout << "Plus3 work done. Var = " << var << std::endl; 
        plus_lock.unlock(); 
        } else { 
        plus_lock.unlock(); 
        return; 
        } 
        this_thread::sleep_for (std::chrono::seconds(3)); 
    } 

}
int main() {

    thread a(plus3) ; 
    thread b(minus1) ;  // creation of the 3 threads ; 
    thread c(check10) ;

    a.join(); 
    b.join(); // wait the endind of the threads ; 
    c.join(); 

    return 0 ; 

}