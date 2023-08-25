/*
Write a small C++ example where a thread “admin” initializes an integer variable var to 10 and 
then waits 3 other threads, which each adds a random number between 0 and 5 to var. The 
program terminates either when all threads finish or when var becomes equal or greater to 15: in 
both cases the “admin” thread is awakened and prints the final value. Write the code of the 
function admin_f() and adder_f() so that the provided main works correctly. 
You should use locks and condition variables. 
Note: if you do not remember the exact syntax of a C++ class, write down a version that likely 
resemble what you remember together some "// comment" briefly summarizing what you were 
willing to use. 

*/
#include <iostream> 
#include <thread> 
#include <vector> 
#include <mutex> 
#include <condition_variable> 
#include <queue> 
#include <fstream> 

using namespace std ; 

std::mutex m; 
std::condition_variable adminCV, adderCV; 
std::queue<int> taskQueue; 
int var = 0; 


void admin_f(){ 
    unique_lock<mutex> admin_lock{m} ; 
    var = 10 ;
    cout << "initialized to 10 " << endl ; 
    adderCV.notify_all() ; 
    adminCV.wait(admin_lock) ; 

    cout  << "Program terminating since var is now : " << var << endl ;
    return ; 


} 
void adder_f() { 
    unique_lock<mutex> adder_lock{m} ; 
    if(var < 10 ){
        adderCV.wait(adder_lock) ;  
    }
    if(var > 15) {
        adminCV.notify_one() ; 
        return ; 
    }
    // gen random number
    int num = rand() % 5 ; 
    var += num ; 
    cout << "i added " << num << "to VAR and now i have " << var << endl ; 
    return ; 
} 


int main() { 
 int var=0; 
 std::vector<std::thread> adders; 
 std::thread admin(admin_f); 
 for(int i = 0; i < 3; i++){ 
 srand((unsigned)time(NULL)); //makes seed different for calling rand() 
 adders.emplace_back(std::thread(adder_f)); 
} 
 for(auto& i : adders) { 
 i.join(); 
 } 
 adminCV.notify_one(); 
 admin.join(); 
 return 0; 
} 