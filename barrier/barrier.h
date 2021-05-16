#include <stdio.h>
#include <pthread.h>
#include <assert.h>

typedef struct  __ordered_threads {
	int cap, barr, active;
	int *flag;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} order_t;

typedef struct __wrapper_args {
	int tid;
	order_t *seq;
	void *(*routine)(void *);
	void *arg;
} wrap;

void *starter(void *arg) {
	wrap *kwarg = (wrap*)arg;

	order_t *seq = kwarg->seq;
	void *(*routine)(void *) = kwarg->routine;

	pthread_mutex_lock(&seq->mutex);
	while(seq->active == 0)
		pthread_cond_wait(&seq->cond, &seq->mutex);

	pthread_mutex_unlock(&seq->mutex);
	routine(arg);
	return NULL;
}

void initialize(order_t* seq, int cap) {
	seq->cap = cap;
	seq->active = 0;
	seq->barr = 0;
	seq->flag = (int *)malloc(cap * sizeof(int));

	for(int i=0; i<cap; i++)
		seq->flag[i] = 0;

	assert(pthread_mutex_init(&seq->mutex, NULL) == 0);
	assert(pthread_cond_init(&seq->cond, NULL) == 0);
	printf("Initialized\n");
}

void execute(order_t* seq) {
	pthread_mutex_lock(&seq->mutex);
	printf("Started Execution\n");
	seq->active = 1;
	pthread_cond_broadcast(&seq->cond);
	pthread_mutex_unlock(&seq->mutex);
}

void mythread_create(order_t *seq, pthread_t* threads, wrap* kwarg) {
	int cap = seq->cap;

	for(int i=0; i<cap; i++) {
		kwarg[i].seq = seq;
		kwarg[i].tid = i;
 		pthread_create(&threads[i], NULL, starter, &kwarg[i]);
	}
}

void mythread_join(order_t *seq, pthread_t* threads) {
	int cap = seq->cap;

	for(int i=0; i<cap; i++)
		pthread_join(threads[i], NULL);
}

void barrier(order_t *seq, int tid) {
	pthread_mutex_lock(&seq->mutex);
	seq->flag[tid] = 1;
	int cap = seq->cap;
	int barr = 1;

	for(int i=0; i<cap; i++) {
		if(seq->flag[i] == 0)
			barr = 0; // barrier not crossed
	}

	seq->barr = barr;
	while(seq->barr == 0)
		pthread_cond_wait(&seq->cond, &seq->mutex);
	
	pthread_cond_broadcast(&seq->cond);
	pthread_mutex_unlock(&seq->mutex);
}