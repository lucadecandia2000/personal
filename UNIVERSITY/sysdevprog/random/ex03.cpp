/*
Two parallel cyclic threads P1 and P2 must print only one of the following 4 strings at every 
cycle (and then restart with the next cycle/string): 
ABCDE 
ABCED 
BACDE 
BACED 
*/



// SOUTION ->  there are 2 (or more ) possible solutions, one with counters and one that only uses semaphores ;

int nab=0, nde=0; // these are counters 
sem_init (&sab, 0, 1); //Not shared, init at 1 
sem_init (&sc, 0, 0); //Not shared, init at 0 
sem_init (&sde, 0, 0); //Not shared, init at 0 


P1 () { 
    while (TRUE) { 
        sem_wait(sab); // used to print A or B 
        printf(”A”); 
        if (nab==0) { 
        nab++; 
        sem_post(sab); 
        } else { 
        sem_post (sc); 
        } 
        sem_wait(sc); 
        printf(”C”); 
        sem_post(sde); // D and E have the same semapohore since both are ok to run before the other 
        sem_wait (sde); // need to wait on the same sem cause we need to print both E and D 
        printf(”D”); 
        if (nde==0) { 
        nde++; 
        sem_post(sde);  // same check as before 
        } else { 
        printf(”\n”); 
        nab = nde = 0; 
        sem_post(sab);  // start again 
        } 
    } 
} 


P2 () { 
    while(TRUE) { 
        sem_wait(sab); 
        printf(”B”); 
        if (nab==0) {   // same checking of P1 --> if this is the first print of A or B then increment the counter and post on the same semaphore ; 
        nab++; 
        sem_post(sab); 
        } else { 
        sem_post (sc); // post on the other semaphore of C cause the 3rd element to be printed is always C 
        } 
        sem_wait (sde); 
        printf(”E\n”); 
        if (nde==0) { 
        nde++; 
        sem_post(sde); 
        } else { 
        printf(”\n”); 
        nab = nde = 0; 
        sem_post(sab); 
        } 
    } 
} 