/*
Write a C++ program for simulating the control of an elevator in a building of two floors, according to the 
following man requirements: 
 - the elevator can be called by a person from ground floor (= floor 0), from floor 1 or from floor 
 2: when it is called from a floor different from the one the elevator is currently waiting, it 
 should firstly move to that floor to pick the person up; 
 - once arrived at the calling floor, the elevator will receive an order to go to a different floor 
 (make it random but consistent with its current status, i.e. if elevator is at floor 2, request 
 should be for 0 or 1). 
The following simplifications are allowed: 
 -elevator’s trip time is assumed to be constant (set it to 2 seconds); 
 -elevator has no memory: it will not queue requests that it cannot serve, which are discarded 
 (it is equivalent to a person taking the stairs if elevator is busy); 
 -new requests to the elevator arrive between 1 and 3 seconds; 
 -simulation runs for 30 seconds; 
The function to put a thread in the sleep status (e.g., for 1 second) is the following 
on  e: std::this_thread::sleep_for (std::chrono::seconds(1)) 
Write the code of the program and manage threads synchronization. 
Make sure all threads finish running before the main program terminates. 
*/
using namespace std ; 

mutex m ; 
condition_variable cv ; 

int floor = 0 ; 
int request_from = 1 ;  // all initialization 
int floor_requested = 2  ;
bool simulation_on = true;
bool elevator_free = true;


void timer() { // instead of this thread we could use a while in the main starting from 30 and decrementing 1sec for each iteration but this i probably a better solution 
        
    this_thread::sleep_for(chrono::seconds(30));  // simulation lasts 30 seconds
    unique_lock<mutex> end_lk{m};   // need the lock to turn off the simulation 
    simulation_on=false;  // lock destroyed after this cause it will go out of scope !!

}

void request() {
  while(true) {
   
    unique_lock<mutex> lock{m} ; 
    
    if (!simulation_on) {
        cv.notify_one();
        break;
    }
    if (elevator_free) {

        floor_requested = make_proper_request(floor); // this funcion is not actually implemented and it makes a request from a floor different from the one where the elevator is currently 
        request_from = make_proper_request(floor_requested);

        cout << "Requested floor " <<floor_requested << " from floor " << request_from << endl;

        int r = rand() % 3 + 1; // random number as the requests arrive every 1 to 3 seconds 
        lock.unlock();  // request has been served 
        cv.notify_one(); // signal to the elevator that a new request has arrived 
        this_thread::sleep_for(chrono::seconds(r));
        }
    }


  }


void elevator() {

       while(simulation_on) {

        unique_lock<std::mutex> lock{m};
        elevator_free=false;  // set the flag to false so that other requests are not created ; 

        if(floor!=request_from) {
            cout << ">> PICKING PERSON >> Elevator going from floor " << floor << " to floor "
                      << request_from << endl;
            floor = request_from;
            this_thread::sleep_for(std::chrono::seconds(2)); // given time to serve a request ; 
        }

        cout << ">> SERVING PERSON >> Elevator going from floor " << floor << " to floor " << floor_requested << endl;
        floor = floor_requested; // we get to the wanted floor ; 
        this_thread::sleep_for (std::chrono::seconds(2));
        elevator_free=true; // re-set the elevator to free , as in ready to recieve new requests ; 
        cv.wait(lock); // go in wait state for new requests, releasing the lock ; 
    }

}

int main () {
    cout << "Program is starting" << endl ; 

    thread t(timer) ; 
    thread r(request) ;  // creation of the threads ; 
    thread e(elevator) ; 

    t.join() ; 
    r.join() ; // join all to terminate the program ; 
    e.join() ;

    cout << "Program is finished " << endl ; 

    return 1 ; 
 


}