/*  A function f executes two critical sections A and B in sequence, following the scheme represented 
by the following piece of code: 
void f() { 
 A; 
 B; 
} 
Function f is executed just once by N different threads, and all threads have to wait for each other 
after section A is executed and before executing section B. Explain which is the code that is 
necessary to insert between section A and section B to obtain that behavior. 
*/

// SOLUTION 1

// all threads have to wait on a barrier after the execution of A, so we need a barrier initialized to N elements  

// first solution (using POSIX LIBRARIES) 

pthread_barrier_t barrier; // create a variable barrier
pthread_barrier_init(barrier, N); // initialize it to N 
A;   // execute A critical section 
pthread_barrier_wait(barrier); // wait on barrier 
B; // will be executed after the barrier releases the N threads 


// other solution could be to implement barriers with semaphores and counters 

// we might need to take into consideration the possiblity of a "fast" thread which might activate the barrier before other threads arrive 
// in that case we could use a double barrier ;



