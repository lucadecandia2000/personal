/*
Write a small C++ example where a thread “admin” initializes an integer variable var to 10 and 
then waits 3 other threads, which each adds a random number between 0 and 5 to var. The 
program terminates either when all threads finish or when var becomes equal or greater to 15: in 
both cases the “admin” thread is awakened and prints the final value. Write the code of the 
function admin_f() and adder_f() so that the provided main works correctly
*/

// Solution using locks and condition variables 

#include <iostream> 
#include <thread> 
#include <vector> 
#include <mutex> 
#include <condition_variable> 
#include <queue> 
#include <fstream>

using namespace std ;

std::mutex m; // will be used for mutual exclusion 
std::condition_variable adminCV, adderCV;  // we need to use 2 CV cause the admin needs to be awaken in the end, we might also use only one but it would mean 
                                           // complicating our life with more useless checks on the single CV 
int var = 0; 

void a_f() {
    std::unique_lock<std::mutex> admin_lock{m}; // RAII wrapper, wont need to destroy release it
    var = 10;
    adderCV.notify_all() ; 
    adminCV.wait() ; // now it is time fo the adders to add their value and later to awaken the admin thread 

    // now time to print
    cout << "Value = " << var << endl ; 

}

void adder_f() {
    std::unique_lock<std::mutex> adder_lock{m}; // gets the lock on the mutex 
    if (var == 0) { 
    adderCV.wait(adder_lock); // it means that var has not been initialized yet by the admin thread, need to wait(while waiting release the lock)
    }
    if (var >= 15) { 
        // if var is over the threshold, notify admin and exit 
        adminCV.notify_one(); 
        return; 
    } 

    int n = rand() % 5; 
    var += n;  
    return; 

}

int main () {
    
 vector<thread> adders  ; // we need 3 adder threads 
 thread admin(a_f) // admin thread will execute the admin function

 for(int i = 0; i < 3; i++){ 
 srand((unsigned)time(NULL)); //makes seed different for calling rand() 
 adders.emplace_back(std::thread( adder_f)); 
 }   

 for(auto& i : adders) { 
 i.join(); // need to join on the adder threads 
 }

 adminCV.notify_one();  // notify the ADMIN condition variable 
 admin.join(); wait on admin thread 
 return 0; 

}