#include <iostream>
#include <stdio.h>
#include <time.h>
#include "MatrixMultiplication_ispc.h"
#define N 8
#define MAX_SIZE 2000
using namespace std;

int s[N] = { 50, 100, 150, 300, 500, 800, 1200, 2000 };

int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int r[MAX_SIZE][MAX_SIZE];

int main(int argc, char *argv[]) {
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
		ispc::multiply(a, b, r, n);
		clock_t finish = clock();

		/* print results */
		cout << "size = " << n << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
	}

	getchar();

	return 0;

}