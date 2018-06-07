#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#define N 7

int num_steps[N] = { 1000000, 5000000, 10000000, 50000000, 100000000, 500000000, 1000000000 };

using namespace std;

int main(int argc, char *argv[]) {
	int nprocs;
	int myid;
	double start_time,end_time;
	double x,pi;
	double sum = 0.0;
	
	/* initialize for MPI*/
	MPI_Init(&argc, &argv);  // starts MPI

	/* get number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	/* get this process's number (ranges from 0 to nprocs - 1) */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	for (int i = 0; i < N; i++) {
		int n = num_steps[i];
		
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		double step = 1.0 / (double)n;
		
		clock_t start = clock();
		
		/* do computation */
		for (int i = myid; i < n; i+=nprocs) {
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}
		sum = step * sum;
		MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  // added
		
		/* print results */
		if (myid == 0) {
			clock_t finish = clock();
			cout << "NUM_STEPS = " << n << endl;
			cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl<< endl;
		}
		
	}
	
	/* clean up for MPI */
	MPI_Finalize();
	
	return 0;
}
