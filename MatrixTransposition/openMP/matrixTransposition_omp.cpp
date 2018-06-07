#include <omp.h>
#include <iostream>
#include <time.h>
#define MAX_SIZE 15000
#define N 7 

using namespace std; 

int size[N] = { 500, 1000, 1500, 3000, 5000, 8000, 15000 };
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];

int main() {
	int n;
	
	for (int i = 0; i < N; i++) {
		n = size[i];
		// 初始化 
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
			    a[j][k] = j * n + k;
			}
		}
		
		clock_t start = clock(); 
		
		//计算转置 
		#pragma omp parallel for schedule(runtime) shared(a, b)
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				b[k][j] = a[j][k];
			}
		}
		
		clock_t finish = clock(); 
		
		/* print results */
		cout << "size = " << n << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
	}
	return 0; 
} 
