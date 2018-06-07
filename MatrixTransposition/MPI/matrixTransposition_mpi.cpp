#include <mpi.h>
#include <iostream>
#include <time.h>
#define MAX_SIZE 15000
#define N 7

using namespace std; 

int size[N] = { 500, 1000, 1500, 3000, 5000, 8000, 15000 };
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];

MPI_Status status;

int main(int argc,char **argv){  
    int nprocs, myid;
    int aveRow;
	int extraRow;
	int offset;
	int rows;
	int n;
	
    /* initialize for MPI*/
	MPI_Init(&argc, &argv);  // starts MPI
	/* get number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	/* get this process's number (ranges from 0 to nprocs - 1) */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  
    for (int i = 0; i < N; i++) {
    	n = size[i];
    	
    	if (myid == 0) {
			
			for (int j = 0; j < n; j++) {
			    for (int k = 0; k < n; k++) {
			    	a[j][k] = j * n + k;
				}
			}
			
			aveRow = n / (nprocs - 1);
			extraRow = n % (nprocs - 1);
			offset = 0;
			
			clock_t start = clock();
			
			for (int dest = 1; dest < nprocs; dest++) {
				rows = (dest <= extraRow) ? aveRow + 1 : aveRow;
				MPI_Send(&offset,1,MPI_INT,dest,1,MPI_COMM_WORLD);
				MPI_Send(&rows,1,MPI_INT,dest,1,MPI_COMM_WORLD); 
				for (int j = 0; j < rows; j++) {
					MPI_Send(&a[offset + j][0],n,MPI_INT,dest,1,MPI_COMM_WORLD);
				}
				offset += rows;
			}
			
			for (int s = 1; s < nprocs; s++) {
				MPI_Recv(&offset,1,MPI_INT,s,2,MPI_COMM_WORLD,&status);
				MPI_Recv(&rows,1,MPI_INT,s,2,MPI_COMM_WORLD,&status);

				for (int j = 0; j < n; j++) {
					MPI_Recv(&b[j][offset],rows,MPI_INT,s,2,MPI_COMM_WORLD,&status);
				}
			}
			
			clock_t finish = clock();

			/* print results */
			cout << "size = " << n << endl;
			cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
			
	    } else {
	        
	        MPI_Recv(&offset,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			MPI_Recv(&rows,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			for (int j = 0; j < rows; j++) {
				MPI_Recv(&a[j][0],n,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			}
			
			for (int j = 0; j < rows; j++) {
				for (int k = 0; k < n; k++) {
					b[k][offset + j] = a[j][k];
				}
			}
	        
	        MPI_Send(&offset,1,MPI_INT,0,2,MPI_COMM_WORLD);  
		    MPI_Send(&rows,1,MPI_INT,0,2,MPI_COMM_WORLD);  
	        for (int j = 0; j < n; j++) {
				MPI_Send(&b[j][offset],rows,MPI_INT,0,2,MPI_COMM_WORLD);
			}
	    }
	}
    
    MPI_Finalize();  
}  
