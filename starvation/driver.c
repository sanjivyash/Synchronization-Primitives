#include <unistd.h>
#include "starvation.h"

void *simplex(void *arg) {
	int *pos = arg;
	printf("Ran thread #%d\n", *pos);
	return NULL;
}

int main(){
	int cap;
	printf("Enter number of threads: ");
	scanf("%d", &cap);

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
