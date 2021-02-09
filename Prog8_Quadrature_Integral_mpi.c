#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>

double f(double x){
    double pi = 3.141592653589793;
    return (double)(50/(pi*(2500*x*x + 1)));
}
int main(int argc,char **argv){
  double a=0.0,b=10.0;
  int n=10000000;
  double exact=0.499363381076456;
  MPI_Status status;
  int rank,p,i;
  int target,tag;
  double time,error;
  double my_a,my_b,my_n,x;
  double total=0.0,my_total=0.0;
  int src;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);
  if(rank==0)
  {
    time=MPI_Wtime();
    my_n=n/(p-1);
    n=(p-1)*my_n;
  }
  MPI_Bcast(&my_n,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  if(rank==0){
    for(int i=1;i<p;i++){
      my_a=((p-i)*a +(i-1)*b)/(double)(p-1);
      my_b=((p-i-1)*a +(i*b))/(double)(p-1);
      target=i;//process to which we send a and b
      tag=1;
      MPI_Send(&my_a,1,MPI_DOUBLE,target,tag,MPI_COMM_WORLD);
      target=i;tag=2;
      MPI_Send(&my_b,1,MPI_DOUBLE,target,tag,MPI_COMM_WORLD);
    }
    total=0.0;
    my_total=0.0;
  }
  else{//workers
    tag=1;src=0;
    MPI_Recv(&my_a,1,MPI_DOUBLE,src,tag,MPI_COMM_WORLD,&status);
    tag=2;src=0;
    MPI_Recv(&my_b,1,MPI_DOUBLE,src,tag,MPI_COMM_WORLD,&status);
    my_total=0.0;
    for(int i=1;i<my_n;i++){
      x=((my_n-i)*my_a + (i-1)*my_b)/(double)(my_n-1);
      my_total+=f(x);
    }
    my_total=(my_b-my_a)*my_total/(double)(my_n);
    printf("Process %d contribution= %f\n",rank,my_total);
  }
  MPI_Reduce(&my_total,&total,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
  if(rank==0){
    error=fabs(exact-total);
    time=MPI_Wtime()-time;
    printf("Error = %f\nTime Taken= %f\n",error,time);
  }
  MPI_Finalize();
  return 0;
}
