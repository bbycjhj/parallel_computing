#include <iostream>
#include <pthread.h>
#include <time.h>
#define THREAD_NUM 8
#define N 7

using namespace std;

int num_steps[N] = { 1000000, 5000000, 10000000, 50000000, 100000000, 500000000, 1000000000 };

double x,pi;
double sum_s;
double step;
int NumStep;
pthread_mutex_t mutex;

void *calculatePi(void *t) {
	long long id = (long long)t;
	double sum = 0.0;
	for (int i = id; i < NumStep; i+=THREAD_NUM) {
		x = (i + 0.5) * step;
		sum += 4.0 / (1.0 + x * x);
	}
	pthread_mutex_lock(&mutex);
	sum_s += sum;
	pthread_mutex_unlock(&mutex);
	return 0;
}

int main() {
	for (int i = 0; i < N; i++) {
		sum_s = 0.0;
		NumStep = num_steps[i];
		step = 1.0 / (double)num_steps[i];
		long long t[THREAD_NUM] = {0,1,2,3,4,5,6,7};
		pthread_t threads[THREAD_NUM];
		pthread_attr_t attr;
		pthread_mutex_init(&mutex, NULL);
		
		/* For portability, explicitly create threads in a joinable state */
		clock_t start = clock();
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		for (int i = 0; i < THREAD_NUM; i++) {
			pthread_create(&threads[i], &attr, calculatePi, (void *)t[i]);
		}
		
		/* Wait for all threads to complete */
		for (int i = 0; i < THREAD_NUM; i++) {
	   		pthread_join(threads[i], NULL);
		}
		
		pi = step * sum_s;
		
		clock_t finish = clock();
		
		cout << "NUM_STEPS = " << NumStep << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl<< endl;
		
		/* Clean up and exit */
		pthread_attr_destroy(&attr);
	}
}
