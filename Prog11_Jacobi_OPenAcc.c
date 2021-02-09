#include<math.h>
#include<string.h>
#include<openacc.h>
#include<sys/time.h>
#include<stdio.h>
#define N 1024
#define M 1024
float A[N][M];
float Anew[N][M];
int main(int argc,char **argv){
  int i,j;
  const int n=N;
  const int m=M;
  const int iter_max=1000;
  const double tol=1.0e-6;
  double error=1.0;
  struct timeval time;
  double t1,t2;
  memset(A,0,n*m*sizeof(float));
  memset(Anew,0,n*m*sizeof(float));
  for(j=0;j<n;j++){
    A[j][0]=1.0;
    Anew[j][0]=1.0;
  }
  printf("Jacobi relaxation calculation %dx%d mesh\n",n,m);
  gettimeofday(&time,NULL);
  t1=time.tv_sec+(time.tv_usec/1000000.0);
  int iter=0;
  #pragma acc data copy(A,Anew)
  while(error > tol && iter < iter_max)
  {
    error=0.0;
    #pragma acc parallel loop reduction(max:error)
    for(j=1;j<n-1;j++){
      for(i=1;i<m-1;i++){
        Anew[j][i]=0.25*(A[j][i-1]+A[j][i+1]+A[j-1][i]+A[j+1][i]);
        error=fmax(error, fabs (Anew[j][i]-A[j][i]) );
      }
    }
    #pragma acc parallel loop
    for(j=1;j<n-1;j++){
      for(i=1;i<m-1;i++){
        A[j][i]=Anew[j][i];
      }
    }
    if(iter%100==0)
      printf("%5d  %0.6f\n",iter,error);
      iter++;
  }
  gettimeofday(&time,NULL);
  t2=time.tv_sec+(time.tv_usec/1000000.0);
  printf("Total Time :%f secs\n",t2-t1);
  return 0;
}
