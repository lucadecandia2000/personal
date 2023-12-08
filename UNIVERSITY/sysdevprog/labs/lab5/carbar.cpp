#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>

#define DURATION 60 
#define SIZE 5

using namespace std ; 

void * increment(mutex &m , int &time, int &occupied, mt19937 &mt) {
    
    std::uniform_int_distribution<unsigned int> dist(1, 3);

    bool stop = false ; 
    do {

        if(occupied < SIZE){
            lock_guard<mutex> lock(m) ; 
            occupied++;
            cout << "new car" << endl ; 
        }
        int wait = dist(mt) ; 
        if((time + wait) < DURATION) {
            std::this_thread::sleep_for (std::chrono::seconds(wait));
        }else {
            stop = true ; 
        }

    }while(!stop);
    
}

void * decrement(mutex &m, int &time, int &occupied , mt19937 &mt) {
    
    std::uniform_int_distribution<unsigned int> dist(4, 7);

    bool stop = false; 
    do{
        int wait = dist(mt) ; 
        if((time + wait) > DURATION){
            stop = true ; 
        }else {
            std::this_thread::sleep_for (std::chrono::seconds(wait));
        }
        if(occupied>0){
            lock_guard<mutex> lock(m);
            occupied-- ; 
            cout << "car left" << endl ; 
        }
        
    }while(!stop) ;


}

int main() {
    int time = 0 ; 
    int occupied = 0 ; 
    mutex m ; 

    std::random_device rd;           // Inizializza un generatore di numeri casuali
    std::mt19937 mt(rd());          // Utilizza il generatore Mersenne Twister

    thread t1(increment,ref(m), ref(time), ref(occupied), ref(mt)) ; 
    thread t2(decrement, ref(m), ref(time), ref(occupied), ref(mt)) ; 

    do{
        
        std::this_thread::sleep_for (std::chrono::seconds(1));
        time++;

    }while(time < DURATION) ; 

    return 1 ; 

}