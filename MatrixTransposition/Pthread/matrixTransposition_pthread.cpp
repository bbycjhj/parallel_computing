#include <iostream>
#include <pthread.h>
#include <time.h>
#define THREAD_NUM 8
#define MAX_SIZE 15000
#define N 7 

using namespace std; 

int size[N] = { 500, 1000, 1500, 3000, 5000, 8000, 15000 };
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int n;

void *transpose(void *t) {
	long long id = (long long)t;
	for (int j = id; j < n; j+=THREAD_NUM) {
		for (int k = 0; k < n; k++) {
			b[k][j] = a[j][k];
		}
	}
}

int main() {
	
	for (int i = 0; i < N; i++) {
		n = size[i];
		long long t[THREAD_NUM] = {0,1,2,3,4,5,6,7};
		pthread_t threads[THREAD_NUM];
		pthread_attr_t attr;
		// ³õÊ¼»¯ 
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
			    a[j][k] = j * n + k;
			}
		}
		
		/* For portability, explicitly create threads in a joinable state */
		clock_t start = clock();
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		for (int i = 0; i < THREAD_NUM; i++) {
			pthread_create(&threads[i], &attr, transpose, (void *)t[i]);
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
