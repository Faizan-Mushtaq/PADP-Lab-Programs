#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#define SEED 3655942
int main(int argc,char **argv){
  int it=10000,rank,i;
  int red_count,red_it,count=0;
  double x,y,z,pi,t;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0){
    t=MPI_Wtime();
  }
  if(rank>0){
  srand(SEED+rank*10);
  for(i=0;i<it;i++){
    x=(double)rand()/RAND_MAX;
    y=(double)rand()/RAND_MAX;
    z= x*x+y*y;
    if(z<=1) count++;
  }
  }
  MPI_Reduce(&count,&red_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  MPI_Reduce(&it,&red_it,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  red_it=red_it-it;
  if(rank==0){
    t=MPI_Wtime()-t;
    pi=(double)red_count/(double)red_it*4;
    printf("pi: %lf time %lf",pi,t);
  }
  MPI_Finalize();
  return 0;
}
