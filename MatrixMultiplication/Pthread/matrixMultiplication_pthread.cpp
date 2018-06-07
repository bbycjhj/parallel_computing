#include <iostream>
#include <pthread.h>
#include <time.h>
#define THREAD_NUM 8
#define MAX_SIZE 2000
#define N 8
using namespace std; 

int size[N] = { 50, 100, 150, 300, 500, 800, 1200, 2000 };
int n;
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int r[MAX_SIZE][MAX_SIZE];

void *multiply(void *t) {
	long long id = (long long)t;
	for (int i = id; i < n; i += THREAD_NUM) {
	    for (int j = 0; j < n; j++) {
	        r[j][i] = 0;
	        for (int k = 0; k < n; k++) {
	            r[j][i] += a[j][k] * b[k][i];
	        }
	    }
	}
}

int main(int argc, char *argv[]){
    for (int i = 0; i < N; i++) {
    	n = size[i];
    	int count = 1;
		long long t[THREAD_NUM] = {0,1,2,3,4,5,6,7};
		pthread_t threads[THREAD_NUM];
		pthread_attr_t attr;
		
		for (int j = 0; j < n; j++) {
	    	for (int k = 0; k < n; k++) {
	    		a[j][k] = count;
	    		b[j][k] = count;
			}
		}
    	
    	/* For portability, explicitly create threads in a joinable state */
		clock_t start = clock();
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		for (int i = 0; i < THREAD_NUM; i++) {
			pthread_create(&threads[i], &attr, multiply, (void *)t[i]);
		}
		
		/* Wait for all threads to complete */
		for (int i = 0; i < THREAD_NUM; i++) {
		   	pthread_join(threads[i], NULL);
		}
		
		clock_t finish = clock();
		
		/* print results */
		cout << "size = " << n << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
		
		/* Clean up and exit */
		pthread_attr_destroy(&attr);
	}
    return 0;  
  
}  
