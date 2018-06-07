#include <iostream>
#include <omp.h>
#include <time.h>
#define N 7

int num_steps[N] = { 1000000, 5000000, 10000000, 50000000, 100000000, 500000000, 1000000000 };

using namespace std;

int main() {
	for (int i = 0; i < N; i++) {
		double x,pi;
		double sum = 0.0;
		double step = 1.0 / (double)num_steps[i];
		
		/* do computation -- using all available threads */
		clock_t start = clock();
		
		#pragma omp parallel for private(x) reduction(+:sum)
		for (int j = 0; j < num_steps[i]; ++j) {
			x = (j + 0.5) * step;
			sum += 4.0 / (1.0 + x * x);
		}
		pi = step * sum;
		
		clock_t finish = clock();
		
		cout << "NUM_STEPS = " << num_steps[i] << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl<< endl;
	}
}
