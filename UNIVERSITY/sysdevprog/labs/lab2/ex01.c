#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>

static void *my_malloc (int, int);
static void *my_free (void *);
static void *thread_f (void *);

// structure of threads, contains the n(passed initially from the command line and updated at every step), size and v, list of the identifiers and the size of the list
typedef struct thread_s {
  int n;
  int size;
  long int *v;
} thread_t;

int main (int argc, char *argv[]) {
  thread_t thread_d;
  pthread_t tid;

  thread_d.n = atoi (argv[1]);
  thread_d.size = 0;
  thread_d.v = NULL;
    
  pthread_create (&tid, NULL, thread_f, (void *) &thread_d);
  pthread_join (tid, NULL);

  return (1);
}

static void *thread_f (void *arg) {
  thread_t *p, t1, t2;
  pthread_t tid1, tid2;
  int i;
  
  p = (thread_t *) arg;
  if (p->n<=0) {
    for (i=0; i<p->size; i++) {
      fprintf (stdout, "%ld ", p->v[i]); // check if enough have been generated, if so, print and exit 
    }
    fprintf (stdout, "\n");
    pthread_exit ((void *) 1); 
  }
// setup the new thread structure and then create the actual thread 
  t1.n = p->n-1;
  t1.size = p->size+1;
  t1.v = my_malloc (t1.size, sizeof (long int));
  for (i=0; i<p->size; i++) {
    t1.v[i] = p->v[i];
  }
  t1.v[p->size] = pthread_self(); // insert his tid in the list 
  pthread_create (&tid1, NULL, thread_f, (void *) &t1);

  t2.n = p->n-1;
  t2.size = p->size+1;
  t2.v = malloc (t2.size * sizeof (long int));
  for (i=0; i<p->size; i++) {
    t2.v[i] = p->v[i];
  }
  t2.v[p->size] = pthread_self();
  pthread_create (&tid2, NULL, thread_f, (void *) &t2);

  pthread_join (tid1, NULL);
  pthread_join (tid2, NULL);

  t1.v = my_free ((void *) t1.v);
  t2.v = my_free ((void *) t2.v);

  pthread_exit ((void *) 1); 
}

static void * my_malloc (int n, int size) {
  void *p;
  
  p = (void *)  malloc (n * size);
  if (p == NULL) {
    fprintf (stderr, "Allocation error.\n");
    exit (1);
  }

  return (p);
}

static void * my_free (void *p) {
  free (p);
  return (NULL);
}
