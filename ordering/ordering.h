#include <stdio.h>
#include <pthread.h>
#include <assert.h>

typedef struct  __ordered_threads {
	int cap, count, active;
	int* order;
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

	int tid = kwarg->tid;
	order_t *seq = kwarg->seq;
	void *(*routine)(void *) = kwarg->routine;

	pthread_mutex_lock(&seq->mutex);
	while(seq->order[seq->count] != tid || seq->active == 0)
		pthread_cond_wait(&seq->cond, &seq->mutex);

	routine(kwarg->arg);
	seq->count ++;

	pthread_cond_broadcast(&seq->cond);
	pthread_mutex_unlock(&seq->mutex);
	return NULL;
}

void initialize(order_t *seq, int cap, int *order) {
	seq->cap = cap;
	seq->count = 0;
	seq->active = 0;
	seq->order = order;

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

void mythread_create(order_t *seq, pthread_t *threads, wrap *kwarg) {
	int cap = seq->cap;

	for(int i=0; i<cap; i++) {
		kwarg[i].seq = seq;
		kwarg[i].tid = i;
 		pthread_create(&threads[i], NULL, starter, &kwarg[i]);
	}
}

void mythread_join(order_t *seq, pthread_t *threads) {
	int cap = seq->cap;

	for(int i=0; i<cap; i++)
		pthread_join(threads[i], NULL);
}