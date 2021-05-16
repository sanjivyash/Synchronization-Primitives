#include <stdlib.h>
#include "ordering.h"

int* permute(int n) {
	int *r = malloc(n * sizeof(int));

	for(int i=0; i<n; i++)
		r[i] = i;

	for (int i = n-1; i >= 0; --i){
  	int j = rand() % (i+1);
    int temp = r[i];
    r[i] = r[j];
    r[j] = temp;
	}

	return r;
}

void *simplex(void *arg) {
	int *pos = arg;
	printf("Running thread #%d\n", *pos);
	return NULL;
}

int main(){
	int cap;
	printf("Enter the number of threads: ");
	scanf("%d",&cap);

	int arg[cap];
	pthread_t threads[cap];
	int* order = permute(cap);

	for(int i=0; i<cap; i++)
		printf("%d ", order[i]);
	printf("is the random order\n");

	order_t dateko;
	wrap kwarg[cap];
	initialize(&dateko, cap, order);

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
