#include <stdlib.h>
#include "priority.h"

int* arbitrary(int n, int m) {
	int* r = malloc(n * sizeof(int));
	for(int i=0; i<n; i++)
		r[i] = rand()%m;

	return r;
}

void *simplex(void *arg) {
	int *pos = arg;
	printf("Running thread #%d\n", *pos);
	return NULL;
}

int main(){
	int cap, m;
	printf("Enter number of threads and max priority: ");
	scanf("%d %d", &cap, &m);

	int arg[cap];
	pthread_t threads[cap];
	int* priority = arbitrary(cap, m);

	for(int i=0; i<cap; i++)
		printf("(%d %d) ", priority[i], i);
	printf("is the (priority, #thread)\n");

	order_t dateko;
	wrap kwarg[cap];
	initialize(&dateko, cap, priority);

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
