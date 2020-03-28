#include "mpi.h"
#include <stdio.h>

int main(int argc,char *argv[]){
	int myrank,value,size;
	MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    do{
        if (myrank == 0){
            printf("\nPlease give new value=");
            scanf("%d",&value);
            printf("%d  read  <<  (%d)\n",myrank,value);
            if(size > 1){
                MPI_Send(&value,1,MPI_INT,myrank+1,0,MPI_COMM_WORLD);
                printf("%d  send(%d)  -->>  %d\n",myrank,value,myrank+1);
            }
        } else {
            MPI_Recv(&value,1,MPI_INT,myrank-1,0,MPI_COMM_WORLD,&status);
            printf("%d  recv(%d)  <<--  %d\n",myrank,value,myrank-1);
            if(myrank < size-1){
                MPI_Send(&value,1,MPI_INT,myrank+1,0,MPI_COMM_WORLD);
                printf("%d  send(%d)  -->>  %d\n",myrank,value,myrank+1);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }while(value >= 0);

    MPI_Finalize();    
	return 0;
}

