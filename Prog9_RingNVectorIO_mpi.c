#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
void ring_io(int np,int id){
  int dest,i,j,n;
  int ntest[5]={100,1000,10000,100000,1000000};
  int ntestnum=5;
  int src,test,testnum=10;
  MPI_Status status;
  double tave,tmax,tmin,time;
  double*x;
  if(id==0){
    printf("Timing based on %d tests\n",testnum);
    printf("N\t\tTmin\t\tTave\t\tTmax\n");
  }
  for(int i=0;i<ntestnum;i++)
  {
    n=ntest[i];
    x=(double *)malloc(n*sizeof(double));
    if(id==0)
    {
      dest=1;
      src=np-1;
      tave=0;tmin=1.0E+30;tmax=0;
      for(test=1;test<=testnum;test++)
      {
        for(j=0;j<n;j++){
          x[j]=(double)(test+j);
        }
        time=MPI_Wtime();
        MPI_Send(x,n,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
        MPI_Recv(x,n,MPI_DOUBLE,src,0,MPI_COMM_WORLD,&status);
        time=MPI_Wtime()-time;
        tave+=time;
        if(tmin>time) tmin=time;
        if(tmax<time) tmax=time;
      }
      tave=tave/(double)(testnum);
      printf ( "%-8d  %14.6g  %14.6g  %14.6g\n", n, tmin, tave, tmax );
    }
    else{//worker
      src = id - 1;
      dest=((id +1)% np );
      for(test=1;test<=testnum;test++)
      {
        MPI_Recv( x, n, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, &status );
        MPI_Send( x, n, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD );
      }
    }
    free ( x );
  }
  return;
}

int main(int argc,char **argv){
  int id,np;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  if(id==0){
    printf("NO of Process= %d\n",np);
  }
    ring_io(np,id);
    if(id==0){
      printf("Normal End of Execution\n");
    }
  MPI_Finalize();
  return 0;
}
