#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define Rs 3
#define Xs 4
#define Cs 2

typedef struct thread_s {
  int **A;
  int **B;
  int r;
  int x;
  int c;
  int **C;
  int i;
  int j;
} thread_t;

static int **mat_alloc (int, int);
static void mat_init (int **, int, int);
static void mat_mul_seq (int **, int **, int, int, int, int **);
static void mat_mul (int **, int **, int, int, int, int **);
static void *prod_thread (void *);
static void mat_print (int **, int, int);

int main (void) {
  int **A, **B, **C;

  A = mat_alloc (Rs, Xs);
  mat_init (A, Rs, Xs);
  B = mat_alloc (Xs, Cs);
  mat_init (B, Xs, Cs);
  C = mat_alloc (Rs, Cs);

  fprintf (stdout, "Sequential product:\n");
  mat_mul_seq (A, B, Rs, Xs, Cs, C);
  mat_print (C, Rs, Cs);

  fprintf (stdout, "Concurrent product:\n");
  mat_mul (A, B, Rs, Xs, Cs, C);
  mat_print (C, Rs, Cs);

  return (1);
}

static int **mat_alloc (int r, int c) {
  int **m, i;

  m = (int **) malloc (r * sizeof (int *));
  for (i=0; i<r; i++) {
    m[i] = (int *) malloc (c * sizeof (int));
  }
  
  return m;
}

static void mat_init (int **m, int r, int c) {
  int i, j, n;

  n = 1;
  for (i=0; i<r; i++) {
    for (j=0; j<c; j++) {
      m[i][j] = n++;
    }
  }
  
  return;
}

static void mat_mul_seq (int **A, int **B, int r, int x, int c, int **C) {
  int i, j, k;

  for (i=0; i<r; i++)
    for (j=0; j<c; j++) {
      C[i][j] = 0;
      for (k=0; k<x; k++)
        C[i][j] += A[i][k] * B[k][j];
    }

  return;
}

static void mat_mul (int **A, int **B, int r, int x, int c, int **C) {
  pthread_t *th;
  thread_t *ts;
  int i, j;

  th = (pthread_t *) malloc (r * c * sizeof (pthread_t));
  ts = (thread_t *) malloc (r * c * sizeof (thread_t));
  for (i=0; i<r; i++)
    for (j=0; j<c; j++) {
 
      ts[i*c+j].A = A;
      ts[i*c+j].B = B;
      ts[i*c+j].C = C;
      ts[i*c+j].r = r;
      ts[i*c+j].x = x;
      ts[i*c+j].c = c;
      ts[i*c+j].i = i;
      ts[i*c+j].j = j;
      pthread_create (&th[i*c+j], NULL, prod_thread, (void *) &ts[i*c+j]);
    }

  for (i=0; i<r; i++) {
    for (j=0; j<c; j++) {
      pthread_join (th[i*c+j], NULL);
    }
  }

  return;
}

static void *prod_thread (void *arg) { 
  thread_t *ts = (thread_t *) arg;
  int k;

  ts->C[ts->i][ts->j] = 0;
  for (k=0; k<ts->x; k++)
    ts->C[ts->i][ts->j] += ts->A[ts->i][k] * ts->B[k][ts->j];  
 
  return NULL;
}

static void mat_print (int **m, int r, int c) {
  int i, j;
  
  for (i=0; i<r; i++){
    for (j=0; j<c; j++)
      fprintf (stdout, "%d ", m[i][j]);
    fprintf (stdout, "\n");
  }

  return;
}
