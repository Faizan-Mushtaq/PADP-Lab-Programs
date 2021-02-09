#include<math.h>
#include<omp.h>
#include<iostream>
#include<string.h>
using namespace std;
double t=0.0;
inline long strike(bool com[],long i,long stride,long limit)
{
  for(;i<=limit;i+=stride){
    com[i]=true;
  }
  return i;
}
long min(long a,long b){
  return a>b?b:a;
}
long cacheUnfriendly(long n){
  long count=0;
  long m =(long)sqrt((double)n);
  bool*com=new bool[n+1];
  memset(com,0,n);
  t=omp_get_wtime();
  for(long i=2;i<=m;++i){
    if(!com[i]){
      ++count;
      strike(com,2*i,i,n);
    }
  }
  for(long i=m+1;i<=n;++i)
  {
    if(!com[i]){
      ++count;
    }
  }
    t=omp_get_wtime()-t;
    delete[] com;
    return count;
}
long cacheFriendly(long n){
  long count=0;
  long m=(long)sqrt((double)n);
  bool*com=new bool[n+1];
  memset(com,0,n);
  long* factor = new long[m];
	long* striker = new long[m];
	long nf = 0;
  t=omp_get_wtime();
  for(long i=2;i<=m;i++){
    if(!com[i]){
      ++count;
      striker[nf]=strike(com,2*i,i,m);
      factor[nf++]=i;
    }
  }
  for(long w=m+1;w<=n;w+=m)
  {
    long lim=min(w+m-1,n);
    for(long k=0;k<nf;++k)
      striker[k]=strike(com,striker[k],factor[k],lim);
    for(long i=w;i<=lim;++i)
      if(!com[i])
        ++count;
  }
  t=omp_get_wtime()-t;
  delete[] striker;
  delete[] factor;
  delete[] com;
  return count;
}
long ParallelSieve(long n){
  long count=0,m=(long)sqrt((double)n),nf=0;
  long*factor=new long[m];
  t=omp_get_wtime();
  #pragma omp parallel
  {
    bool*com=new bool[m+1];
    long* striker=new long[m];
  #pragma omp single
  {
    memset(com,0,m);
    for(long i=2;i<=m;++i)
      if(!com[i]){
        ++count;
        strike(com,2*i,i,m);
        factor[nf++]=i;
      }
  }
    long base=-1;
    #pragma omp for reduction(+:count)
    for(long w=m+1;w<=n;w+=m)
    {
      memset(com,0,m);
      if(base!=w){
        base=w;
        for(long k=0;k<=nf;++k)
          striker[k]=(base + factor[k] - 1) / factor[k] * factor[k] - base;
        }
        long limit = min(w + m - 1, n) - base;
        for (long k = 0; k < nf; ++k)
				striker[k] = strike(com, striker[k], factor[k], limit) - m;

        for (long i = 0; i <= limit; ++i)
				if (!com[i])
					++count;
			  base += m;
		}
		delete[] striker;
		delete[] com;
	}
    t = omp_get_wtime() - t;
      delete[] factor;
	   return count;
}
int main(){
	long size = 10000,count;
	printf("Size\t\tCache Unfriendly\tCache Friendly\t\tParallel Sieve\n");
	for(int i=1; i<=4; i++ ){
		size = size*10;
		printf("%ld\t",size);
		if(i<3)
			printf("\t");
		count = cacheUnfriendly(size);
		printf("%ld\t%f\t",count,t);
		count = cacheFriendly(size);
		printf("%ld\t%f\t",count,t);
		count = ParallelSieve(size);
		printf("%ld\t%f\n",count,t);
	}
	return 0;
}
