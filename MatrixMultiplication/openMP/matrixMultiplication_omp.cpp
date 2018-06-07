#include <iostream>
#include <omp.h>
#include <time.h>
#define N 8
#define MAX_SIZE 2000
using namespace std; 

int size[N] = { 50, 100, 150, 300, 500, 800, 1200, 2000 };
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int r[MAX_SIZE][MAX_SIZE];

int main(int argc, char *argv[]){
	int n;
	int count = 1;
	
    for (int i = 0; i < N; i++) {
    	n = size[i];
    	
		for (int j = 0; j < n; j++) {
	    	for (int k = 0; k < n; k++) {
	    		a[j][k] = count;
	    		b[j][k] = count;
			}
		}
		
		/* do computation -- using all available threads */
		clock_t start = clock();
			#pragma omp parallel for schedule(runtime) shared(a, b, r)
			for (int k = 0; k < n; k++) {
	            for (int l = 0; l < n; l++) {
	            	r[l][k] = 0;
	                for (int j = 0; j < n; j++) {
	                    r[l][k] += a[l][j] * b[j][k];
	                }
	            }
	        }
		clock_t finish = clock();
		
		/* print results */
		cout << "size = " << n << endl;
		cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
	}
    return 0;  
  
}  
