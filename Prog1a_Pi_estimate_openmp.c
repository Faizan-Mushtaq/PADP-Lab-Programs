#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#define SEED 50123
int main(){
  long n,i,count=0;
  double x,y,z;
  srand(SEED);
  printf("Size\t\tT1\t\tT2\t\tT4\t\tT8\n");
  for(n=10;n<=1000000;n=n*10)
  {
    printf("\n%ld\t",n);
    for(int t=1;t<=8;t+=2){
      count=0;
      double start=omp_get_wtime();
      omp_set_num_threads(t);
      #pragma omp parallel for private(x,y,z)shared(count)
      for(i=0;i<n;i++){
        x=(double)rand()/RAND_MAX;
        y=(double)rand()/RAND_MAX;
        z=x*x + y*y;
        if(z<=1) count++;
      }
      double pi=(double)count/n*4;
      double end=omp_get_wtime()-start;
      printf("%.2lf  %.2lfms \t",pi,end*1000);
    }
  }
  return 0;
}
