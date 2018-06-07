#include <iostream>
#include <mpi.h>
#include <time.h>
#define MAX_SIZE 2000
#define N 8
using namespace std; 

int size[N] = { 50, 100, 150, 300, 500, 800, 1200, 2000 };
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int r[MAX_SIZE][MAX_SIZE];

MPI_Status status;

int main(int argc, char *argv[]){
	int nprocs, myid;
	int count = 1;
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
	    			a[j][k] = count;
	    			b[j][k] = count;
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
				//MPI_Send(&a[offset][0],rows * n,MPI_INT,dest,1,MPI_COMM_WORLD);
				for (int j = 0; j < rows; j++) {
					MPI_Send(&a[offset + j][0],n,MPI_INT,dest,1,MPI_COMM_WORLD);
				}
				//MPI_Send(&b,n * n,MPI_INT,dest,1,MPI_COMM_WORLD);
				for (int j = 0; j < n; j++) {
					MPI_Send(&b[j][0],n,MPI_INT,dest,1,MPI_COMM_WORLD);
				}
				offset += rows;
			}
			
			for (int s = 1; s < nprocs; s++) {
				MPI_Recv(&offset,1,MPI_INT,s,2,MPI_COMM_WORLD,&status);
				MPI_Recv(&rows,1,MPI_INT,s,2,MPI_COMM_WORLD,&status);
				//MPI_Recv(&r[offset][0],rows*n,MPI_INT,s,2,MPI_COMM_WORLD,&status);
				for (int j = 0; j < rows; j++) {
					MPI_Recv(&r[offset + j][0],n,MPI_INT,s,2,MPI_COMM_WORLD,&status);
				}
			}
			
			clock_t finish = clock();

			/* print results */
			cout << "size = " << n << endl;
			cout << "cost time: " << (double)(finish - start) / CLOCKS_PER_SEC << endl << endl;
		
		} else {
			MPI_Recv(&offset,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			MPI_Recv(&rows,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			//MPI_Recv(&a,rows * n,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			for (int j = 0; j < rows; j++) {
				MPI_Recv(&a[j][0],n,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			}
			//MPI_Recv(&b,n * n,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			for (int j = 0; j < n; j++) {
				MPI_Recv(&b[j][0],n,MPI_INT,0,1,MPI_COMM_WORLD,&status);
			}
			
			for (int k = 0; k < n; k++) {
                for (int l = 0; l < rows; l++) {
                	r[l][k] = 0;
                    for (int j = 0; j < n; j++) {
                        r[l][k] += a[l][j] * b[j][k];
                    }
                }
            }
		    MPI_Send(&offset,1,MPI_INT,0,2,MPI_COMM_WORLD);  
		    MPI_Send(&rows,1,MPI_INT,0,2,MPI_COMM_WORLD);  
		    //MPI_Send(&r,rows * n,MPI_INT,0,2,MPI_COMM_WORLD);
		    for (int j = 0; j < rows; j++) {
				MPI_Send(&r[rows][0],n,MPI_INT,0,2,MPI_COMM_WORLD);
			}
	        
		}
	}
	/* clean up for MPI */
  	MPI_Finalize();
    return 0;  
  
}  
