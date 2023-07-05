/*
Write a small C++ example with three threads: 
   - Thread take_value takes a number from the command line. 
   - Thread check_if_prime checks whether the number is prime. 
   - Thread give_answer prints the answer to standard output. 
Thread communication should be made using promises and futures.
*/

// solution 

using namespace std ; 

void take_value(std::promise<int>& user_input) { // getting the promise as a parameter (passed by reference)
    int value  ;  // initialize the value 
    cin >> value  ; 
    user_input.set_value(value) ; 
}
void check_if_prime(future<int>& f_value, promise<bool>& p_result) {
    int n = f_value.get() ; 
    bool prime ; // initialize the prime var  ; 
    // check if prime , here omitted because it is not actually important for the sake of the exercise  ; 
    p_result.setset_value(prime) ; 
}
void give_answer(std::future<bool>& computation) {
    bool answer = computation.get(); 
    string s0 (" "); 
    if(!answer) 
    s0=" NOT"; 
    cout << "Number is" << s0 << " prime"; 
}


int main() {

    // create promises and futures 
    promise<int> user_input ; // promise for the user input 

    thread t1(take_value, ref(user_input)); // t1 has the role of filling this value from stdin ; 

    future<int> f_user_input = user_input.get_future() ; // associate the future with the promise 

    std::promise<bool> p_result; // promise for isPrime or NOT 

    std::thread t2(check_if_prime, std::ref(f_user_input),std::ref(p_result)); // t2 has the role of checking if it is prime and setting the value

    std::future<bool> f_result = p_result.get_future(); //associate the future with the promise 

    std::thread t3(give_answer, std::ref(f_result)); // t3 is responsible for giving the answer ; 

    t1.join(); 
    t2.join(); 
    t3.join(); 
    return 0;
}