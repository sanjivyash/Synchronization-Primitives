#include <stdlib.h>
#include "barrier.h"

void *simplex(void *arg) {
	wrap *kwarg = (wrap *)arg;

	// unwrapping of kwarg
	int *pos = kwarg->arg;
	int tid = kwarg->tid;
	order_t *seq = kwarg->seq;

	printf("Running thread #%d before barrier\n", *pos);
	barrier(seq, tid);
	printf("Running thread #%d after barrier\n", *pos);
	
	return NULL;
}

int main(){
	int cap;
	printf("Enter the number of threads: ");
	scanf("%d",&cap);

	int arg[cap];
	pthread_t threads[cap];

	order_t dateko;
	wrap kwarg[cap];
	initialize(&dateko, cap);

	for(int i=0; i<cap; i++){
		arg[i] = i;
		kwarg[i].arg = &arg[i];
		kwarg[i].routine = simplex;
	}

	mythread_create(&dateko, threads, kwarg);
	execute(&dateko);
	mythread_join(&dateko, threads);

	return 0;
}
