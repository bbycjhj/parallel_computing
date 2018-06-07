#include <iostream>
#include <stdio.h>
#include <time.h>
#include "matrixTransposition_ispc.h"
#define MAX_SIZE 15000
#define N 7 

using namespace std;

int s[N] = { 500, 1000, 1500, 3000, 5000, 8000, 15000 };
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];

int main() {
	for (int i = 0; i < N; i++) {
		int count = 1;
		int n = s[i];

		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				a[j][k] = count;
				b[j][k] = count;
			}
		}

		/* do computation -- using all available threads */
		clock_t start = clock();
		ispc::transpose(a, b, n);
		clock_t finish = clock();

		/* print results */
		cout << "size = " << n << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
	}

	getchar();

	return 0;
}