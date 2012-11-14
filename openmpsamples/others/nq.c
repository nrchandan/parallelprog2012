#include <assert.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 
   How to compile:
   
   This is a mixed OpenMP and Pthreads code, so if you like to play with
   OpenMP part, you need gcc >= 4.2 and compile with gcc -fopenmp nq.c

   Otherwise you can ignore OpenMP and just do gcc nq.c -lpthread
 */

/* SECTION 1.
   ------------------ serial -------------------------------
   serial nqueen is as simple as the following single function.
   nqs(0, 0, 0, 0, n) 
   returns the answer for n queens problem
   --------------------------------------------------------- */
int nqs(unsigned int v, unsigned int l, unsigned int r, int row, int n) 
{
  if (row == n) { 
    return 1;
  } else {
    int c = 0;
    unsigned int b = v | l | r;
    while (b + 1 < (1 << n)) {
      unsigned int o = (b + 1) & (~b);
      c += nqs(v | o, 
	      ((l | o) << 1) & ((1 << n) - 1), 
	      (r | o) >> 1, 
	      row + 1, 
	      n);
      b |= o;
    }
    return c;
  }
}



/* ---------------------------------------------------------
   SECTION 2.
   common setup for parallel version 
   --------------------------------------------------------- */

/* ---------------------------------------------------------
   data structure representing arguments passed to nqueen thread
   (primary reason that this is necessary is pthread_create
   can pass only a single argument to the thread. we make a
   record and pass the pointer to it.
   --------------------------------------------------------- */

typedef struct nqueen_arg {
  pthread_t thid;
  unsigned int v;
  unsigned int l;
  unsigned int r;
  int row;
  int n;
  int parallel_depth;
  int ans;
} nqueen_arg, * nqueen_arg_t;



/* ---------------------------------------------------------
   SECTION 3.
   "naively" parallelized version.
   create a thread for each recursive call up to a certain depth.
   --------------------------------------------------------- */


void * nqp_thread(void * arg);

void fork_nqp(nqueen_arg_t arg,
	      unsigned int v, unsigned int l, unsigned int r, int row, int n,
	      int parallel_depth) {
  /* client calls this instead of the nqueen procedure itself,
     to create the thread that then calls the procedure.
     we need to package arguments into a single record to pass
     it via pthread_create */
  arg->v = v;
  arg->l = l;
  arg->r = r;
  arg->row = row;
  arg->n = n;
  arg->parallel_depth = parallel_depth;
  pthread_create(&arg->thid, (pthread_attr_t *)0, nqp_thread, arg);
}

void * nqp_thread(void * arg_) {
  /* the thread that does the work */
  nqueen_arg_t arg = (nqueen_arg_t)arg_;
  arg->ans = nqp(arg->v, arg->l, arg->r, arg->row, arg->n, 
		 arg->parallel_depth);
  return 0;
}



/* the main procedure of "naively parallelized" nqueen */
int nqp(unsigned int v, unsigned int l, unsigned int r, 
	int row, int n,
	int parallel_depth) 
{
  if (row == n) { 
    return 1;
  } else {
    int n_threads = 0;
    nqueen_arg_t args = 0;
    unsigned int b = v | l | r;
    int c = 0;
    while (b + 1 < (1 << n)) {
      unsigned int o = (b + 1) & (~b);
      if (row < parallel_depth) {
	/* spawn thread up to a certain depth (parallel depth) */
	assert(n_threads < n);
	if (args == 0) args = (nqueen_arg_t)malloc(sizeof(nqueen_arg) * n);
	fork_nqp(args + n_threads,
		 v | o, 
		 ((l | o) << 1) & ((1 << n) - 1), 
		 (r | o) >> 1, 
		 row + 1, 
		 n,
		 parallel_depth);
	n_threads++;
      } else {
	/* or just do it sequentially */
	c += nqp(v | o, 
		 ((l | o) << 1) & ((1 << n) - 1), 
		 (r | o) >> 1, 
		 row + 1, 
		 n, 
		 parallel_depth);
      }
      b |= o;
    }
    /* wait for termination of spawned threads */
    int i;
    for (i = 0; i < n_threads; i++) {
      pthread_join(args[i].thid, (void **)0);
      c += args[i].ans;
    }
    return c;
  }
}



/* ---------------------------------------------------------
   SECTION 4.
   necessary setup for decoupling "work" and "thread"
   --------------------------------------------------------- */

/* data structure representing a queue of work to be done
   (a unit of work is represented by args to nqueen procedures) */
typedef struct work_queue {
  pthread_mutex_t mx;
  int hd;
  int sz;
  int capacity;
  nqueen_arg_t args;
} work_queue, *work_queue_t;

work_queue_t mk_work_queue() {
  int capacity = 1000;
  work_queue_t wq
    = (work_queue_t)malloc(sizeof(work_queue));
  nqueen_arg_t args = (nqueen_arg_t)malloc(sizeof(nqueen_arg) * capacity);
  assert(wq);
  assert(args);
  pthread_mutex_init(&wq->mx, 0);
  wq->hd = 0;
  wq->sz = 0;
  wq->capacity = capacity;
  wq->args = args;
  return wq;
}

void work_queue_add(work_queue_t wq, 
		    unsigned int v, unsigned int l, unsigned int r, 
		    int row, int n, int parallel_depth) {
  int sz = wq->sz;
  int capacity = wq->capacity;
  nqueen_arg_t args = wq->args;
  if (sz >= capacity) {
    int new_capacity = capacity * 2;
    if (new_capacity <= capacity) new_capacity = capacity + 1;
    nqueen_arg_t new_args 
      = (nqueen_arg_t)malloc(sizeof(nqueen_arg) * new_capacity);
    bcopy(args, new_args, sizeof(nqueen_arg) * capacity);
    wq->capacity = capacity = new_capacity;
    wq->args = args = new_args;
  }
  assert(sz < capacity);
  nqueen_arg_t arg = args + sz;
  arg->v = v;
  arg->l = l;
  arg->r = r;
  arg->row = row;
  arg->n = n;
  arg->parallel_depth = parallel_depth;
  wq->sz = sz + 1;
}




/* ---------------------------------------------------------
   SECTION 5.
   main procedure for generating work to be done in parallel.
   it normally recurses up to the specified depth (parallel_depth).
   at parallel_depth, it instead creates a unit of rowk and
   returns immediately.
   --------------------------------------------------------- */

int nqw(work_queue_t wq,
	unsigned int v, unsigned int l, unsigned int r, 
	int row, int n,
	int parallel_depth) 
{
  if (row == n) { 
    return 1;
  } else if (row == parallel_depth) {
    /* return immediately after generating work at this depth */
    work_queue_add(wq, v, l, r, row, n, -1);
    return 0;
  } else {
    int c = 0;
    unsigned int b = v | l | r;
    while (b + 1 < (1 << n)) {
      unsigned int o = (b + 1) & (~b);
      c += nqw(wq,
	       v | o, 
	       ((l | o) << 1) & ((1 << n) - 1), 
	       (r | o) >> 1, 
	       row + 1, 
	       n, 
	       parallel_depth);
      b |= o;
    }
    return c;
  }
}


/* ---------------------------------------------------------
   do generated work in parallel using OpenMP
   --------------------------------------------------------- */
int nqw_omp_master(int n, int parallel_depth) {
  work_queue_t wq = mk_work_queue();
  /* generate work */
  int c = nqw(wq, 0, 0, 0, 0, n, parallel_depth);
  int i;
  /* do generated work in parallel */
#pragma omp parallel for reduction(+:c) schedule(static)
  for (i = 0; i < wq->sz; i++) {
    nqueen_arg_t arg = wq->args + i;
    c += nqw(wq, 
	     arg->v, arg->l, arg->r, arg->row, arg->n, arg->parallel_depth);
  }
  return c;
}


/* ---------------------------------------------------------
   do generated work in parallel using Pthreads.
   it is essentially equivalent to what OpenMP does (with 'dynamic'
   scheduling).
   --------------------------------------------------------- */
void * nq_worker(void * wq_) {
  work_queue_t wq = (work_queue_t)wq_;
  while (1) {
    nqueen_arg_t arg = 0;
    /* try to fetch work */
    pthread_mutex_lock(&wq->mx); 
    {
      int hd = wq->hd;
      if (hd < wq->sz) {
	arg = wq->args + hd;
	wq->hd = hd + 1;
      }
    } 
    pthread_mutex_unlock(&wq->mx);
    if (arg ==0) break;		/* no work left, quit */
    arg->ans 
      = nqw(wq, arg->v, arg->l, arg->r, arg->row, arg->n, arg->parallel_depth);
  }
}

int nqw_pth_master(int n, int parallel_depth) {
  work_queue_t wq = mk_work_queue();
  /* generate work */
  int c = nqw(wq, 0, 0, 0, 0, n, parallel_depth);
  int i;
  /* create pthraeds that do the generated work */
  int n_threads = atoi(getenv("OMP_NUM_THREADS"));
  pthread_t * thids = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
  for (i = 0; i < n_threads; i++) {
    pthread_create(thids + i, (pthread_attr_t *)0, nq_worker, (void *)wq);
  }
  for (i = 0; i < n_threads; i++) {
    pthread_join(thids[i], (void **)0);
  }
  for (i = 0; i < wq->sz; i++) {
    c += wq->args[i].ans;
  }
  return c;
}

/* ---------------------------------------------------------
   SECTION 6.
   allow works to be added at any point
   --------------------------------------------------------- */

void work_queue_add_lock(work_queue_t wq, 
			 unsigned int v, unsigned int l, unsigned int r, 
			 int row, int n, int parallel_depth) {
  /* adding a work now needs a lock */
  pthread_mutex_lock(&wq->mx); 
  work_queue_add(wq, v, l, r, row, n, parallel_depth);
  pthread_mutex_unlock(&wq->mx); 
}

int nqw2(work_queue_t wq,
	unsigned int v, unsigned int l, unsigned int r, int row, int n,
	int parallel_depth) 
{
  if (row == n) { 
    return 1;
  } else if (row <= parallel_depth) {
    /* return immediately after generating work at this depth */
    work_queue_add_lock(wq, v, l, r, row, n, -1);
    return 0;
  } else {
    int c = 0;
    unsigned int b = v | l | r;
    while (b + 1 < (1 << n)) {
      unsigned int o = (b + 1) & (~b);
      c += nqw2(wq,
		v | o, 
		((l | o) << 1) & ((1 << n) - 1), 
		(r | o) >> 1, 
		row + 1, 
		n, 
		parallel_depth);
      b |= o;
    }
    return c;
  }
}

int nqw2_pth_master(int n, int parallel_depth) {
  work_queue_t wq = mk_work_queue();
  int i;
  /* create pthraeds that do works TO BE GENERATED below */
  char * n_threads_s = getenv("OMP_NUM_THREADS");
  assert(n_threads_s);
  int n_threads = atoi(n_threads_s);
  pthread_t * thids = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
  for (i = 0; i < n_threads; i++) {
    pthread_create(thids + i, (pthread_attr_t *)0, nq_worker, (void *)wq);
  }

  /* generate work. threads get started as soon as the work
     enters the queue */
  int c = nqw2(wq, 0, 0, 0, 0, n, parallel_depth);


  for (i = 0; i < n_threads; i++) {
    pthread_join(thids[i], (void **)0);
  }
  for (i = 0; i < wq->sz; i++) {
    c += wq->args[i].ans;
  }
  return c;
}





/* ---------------------------------------------------------
   SECTION 6.
   dispatcher and main procedure
   --------------------------------------------------------- */

int nqueen(char * strategy, int n, int parallel_depth) {
  if (strategy[0] == 's') {
    printf("serial\n");
    return nqs(0, 0, 0, 0, n);
  } else if (strategy[0] == 'n') {
    printf("naive\n");
    return nqp(0, 0, 0, 0, n, parallel_depth);
  } else if (strategy[0] == 'o') {
    printf("omp\n");
    return nqw_omp_master(n, parallel_depth);
  } else if (strategy[0] == 'p') {
    printf("worker by pthread\n");
    return nqw_pth_master(n, parallel_depth);
  } else if (strategy[0] == 'w') {
    printf("worker by pthread2\n");
    return nqw2_pth_master(n, parallel_depth);
  } else {
    printf("no such strategy %s\n", strategy);
    exit(1);
  }
}

double cur_time() {
  struct timeval tp[1];
  gettimeofday(tp, 0);
  return tp->tv_sec + tp->tv_usec * 1.0E-6;
}

int main(int argc, char ** argv) {
  char * strategy = argv[1];
  int n = atoi(argv[2]);
  int p = -1;
  if (argc > 3) p = atoi(argv[3]);
  double t0 = cur_time();
  int count = nqueen(strategy, n, p);
  double t1 = cur_time();
  printf("%.3f sec (%d queen = %d)\n", t1 - t0, n, count);
}
