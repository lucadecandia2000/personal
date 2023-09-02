#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>


struct thread_data{
  char* fileName ; 
  int* output ; 
  int size ; 
} ; 
using namespace std ; 

void * t_f( thread_data *data) {
    ifstream fileInput(data->fileName, ios::binary);

    if(!fileInput.is_open()){
    cout << "Error opening file: " << data->fileName << endl;
    exit(0);
    }

    fileInput >> data->size ;

    data->output = (int *)  malloc ( data->size * sizeof(int)) ; 

    for (int i = 0 ; i < data->size ; i++) {
        fileInput >> data->output[i] ; 
    }

    sort(data->output , data->output + data -> size ) ; // list of numbers , first and last pointer so i need the last --> beginning + size == end ;;

    exit (1) ; 
}

int main(int argc, char *argv[]) {

vector<thread_data> t_data ; 
vector<thread> threadList ; 
int finalSum = 0 ; 

if (argc < 3 ) {
    cout << "Not enough parameter passed" << endl ;
    exit(1) ;  
}

for(int i = 1; i< argc -1 ; i++) {
  t_data[i-1].fileName = argv[i-1] ; 
  thread t(t_f , &t_data[i] ) ; 
  threadList.emplace_back(move(t)) ; // REMEMBER THREADS CAN ONLY BE MOVED AND NOT COPIED ; 
}

for( int i = 1 ; i < argc -1 ; i++ ){
    threadList[i].join() ; 
}

for ( int i = 1 ; i < argc -1 ; i++ ){
 finalSum += t_data[i].size ; 
}

// alloc the needed space for writing all ;
 int *final = (int *) malloc(finalSum * sizeof(int)) ; 

 for (int i = 1 ; i< argc ; i++) {
    copy (final, final + t_data[i].size, t_data[i].output) ; // not the expected behaviour 
 }

 sort(final, final+finalSum) ; 

return 1 ;
}