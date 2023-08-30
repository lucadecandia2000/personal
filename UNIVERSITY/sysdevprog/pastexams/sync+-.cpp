#include <iostream>
#include <thread>
#include <semaphore.h>

#define N 10

using namespace std;

sem_t semp, semm, sems, semtnl;

void *TP() {
    int count = 0;
    while (1) {
        sem_wait(&semp);
        cout << "+";
        count++;
        if (count == N) {
            count = 0;
            sem_post(&semm);
        }
        else {
            sem_post(&semp);
        }
    }
}

void *TM() {
    int count = 0;
    while (1) {
        sem_wait(&semm);
        cout << "-";
        count++;
        if (count == N) {
            count = 0;
            sem_post(&sems);
        }
        else {
            sem_post(&semm);
        }
    }
}

void *TS() {
    int count = 0;
    while (1) {
        sem_wait(&sems);
        cout << "*";
        count++;
        if (count == N) {
            count = 0;
            sem_post(&semtnl);
        }
        else {
            sem_post(&sems);
        }
    }
}

void *TNL() {
    while (1) {
        sem_wait(&semm);
        cout << endl;
        sem_post(&semp);
    }
}

int main() {
    sem_init(&semp, 0, 1);
    sem_init(&semm, 0, 0);
    sem_init(&sems, 0, 0);
    sem_init(&semtnl, 0, 0);

    thread tp(TP);
    thread tm(TM);
    thread ts(TS);
    thread tnl(TNL);

    // Detach all threads
    tp.detach();
    tm.detach();
    ts.detach();
    tnl.detach();

    // Main thread doesn't need to join or detach, so it will continue running.

    // Optionally, you can add a sleep to let the detached threads run for some time.
    // This is just to demonstrate that they continue running.
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
