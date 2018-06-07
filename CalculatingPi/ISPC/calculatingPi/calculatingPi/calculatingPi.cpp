#include <iostream>
#include <stdio.h>
#include <time.h>
#define N 7

int num_steps[N] = { 100000000, 15000000, 20000000, 25000000, 30000000, 350000000, 400000000 };

// Include the header file that the ispc compiler generates
#include "calculatingPi_ispc.h"

using namespace std;

int main() {
	for (int i = 0; i < N; i++) {
		double result;
		int n = num_steps[i];
		clock_t start = clock();
		ispc::calculatingPi(n, result);
		clock_t finish = clock();

		cout << "NUM_STEPS = " << n << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
	}

	getchar();
}