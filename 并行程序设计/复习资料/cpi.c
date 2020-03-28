#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f(double x){
    return (4.0/(1+x*x));
}

int main(int argc,char *argv[]){
    int done=0,n,myrank,numprocs,i;
    double PI25DT=3.141592653589793238462643;
    double mypi,pi,h,sum,x;
    double startwtime=0.0,endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Get_processor_name(processor_name,&namelen);

    printf("\nProcessor %d of %d on %s",myrank,numprocs,processor_name);
    n = 0;
    if(myrank == 0){
        printf("\nPlease give N=");
        scanf("%d",&n);
        startwtime = MPI_Wtime();
    }

    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    h = 1.0/(double)n;
    sum = 0.0;
    for (i = myrank + 1;i <= n;i += numprocs){
        x = h * ((double)i-0.5);
        sum += f(x);
    }
    mypi = h * sum;
    MPI_Reduce(&mypi,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    if(myrank == 0){
        printf("pi is approximately %.16f, Error is %.16f\n",pi,fabs(pi-PI25DT));
        endwtime = MPI_Wtime();
        printf("wall clock time = %f\n",endwtime-startwtime);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}

