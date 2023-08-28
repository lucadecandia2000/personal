/*
Two files store a list of real values sorted in ascending order. The number of real values stored in each file is 
indicated on top of the file using an integer value. The values of those two files have to be merged into a unique 
list of values (please notice, not re-sorted but simply merged) and stored into a third file with the same format 
but in descending order. 
Write a C function receiving the 3 file names and performing such an operation using memory-mapped I/O. 
Indicate advantages and disadvantages of using memory mapping versus performing standard I/O operations in 
terms of logic, code complexity, efficiency, etc
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>  // for file permission flags
#include <sys/types.h>

int main ( int argc , char *argv[]) {

    int fd1, fd2, fd3 ; 
    int n1, n2, n3 ; 

    void* m1 ; 
    void* m2 ;  
    void* m3 ; 

    if ( argc < 4 ) {
        printf(stderr, "error in passinf vars") ; 
    }

    fd1 = open(argv[1], O_RDONLY , 0) ; 
    fd2 = open(argv[2], O_RDONLY , 0) ;    
    fd3 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0);   

    if( fd1 == -1 || fd2 == -1 || fd3 == -1) {
        printf(stderr, "Error opening the files"); 
    }   
    
    int r = read(fd1, &n1, sizeof(int)) ; 
    // checks on r number returned (should be 1)

    r = read(fd2, &n2, sizeof(int)) ; 

    n3 = n1+n2 ; 

    write(fd3, &n3, sizeof(int)) ; 

    // memory mapping 

    m1 = mmap(0, n1*sizeof(float), PROT_READ, MAP_SHARED,fd1, sizeof(int)); // 0 means up to the compiler , last is offset(keep in mind we already wrote the 1st num)
    m2 = mmap(0, n1*sizeof(float), PROT_READ, MAP_SHARED,fd2, sizeof(int)); 
    m3 = mmap(0, n1*sizeof(float), PROT_WRITE, MAP_SHARED,fd3, sizeof(int)); 

    if ( m1 = MAP_FAILED || m2 == MAP_FAILED || m3 == MAP_FAILED) {
        printf(stderr, "Error in memory mapping") ; 

        exit(1) ; 
    }

    // do the merging and sorting ; 

    // ...

    // unmapping and closing 

    munmap(m1, n1*sizeof(float)); 
    munmap(m2, n2*sizeof(float)); 
    munmap(m3, n3*sizeof(float)); 
    close(fd1); 
    close(fd2); 
    close(fd3); 

}