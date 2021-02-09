#include<stdio.h>
#include<error.h>
#include<gd.h>
#include<omp.h>
#include<string.h>
int main(int argc,char **argv){
  FILE *fp,*fp1={0};
  gdImagePtr img;
  char *iname=NULL;
  char oname[15];
  int color,x,y,i=0;
  int red,green,blue,tmp,tid;
  long w,h;
  color=x=y=w=h=0;
  red=green=blue=0;
  char inputnames[1][15]={"input1.png"};
  omp_sched_t def_sched;
  int def_chunk_size;
  omp_get_schedule(&def_sched,&def_chunk_size);
  printf("Default %d %d \n",def_sched,def_chunk_size);
	printf("Size\t\tDefault\t\tStatic\t\tDynamic\t\tGuided\n");
  for(int i=0;i<1;i++){//if four imgs use 4
    iname=inputnames[i];
    for(int sched=0x0;sched<=0x3;sched++)
    {
      fp=fopen(iname,"r");
      sprintf(oname,"output%d%d.png",i,sched);
      img=gdImageCreateFromPng(fp);
      w=gdImageSX(img);
      h=gdImageSY(img);
      if(sched==0x0){
        printf("%ldx%ld\t",w,h);
        omp_set_schedule(def_sched,def_chunk_size);
      }
      else
        omp_set_schedule(sched,0);
      double t=omp_get_wtime();
      #pragma omp parallel for private(y,color,red,green,blue,tmp,tid)
      for(x=0;x<w;x++){
        for(y=0;y<h;y++){
          tid=omp_get_thread_num();
          color=gdImageGetPixel(img,x,y);
          red=gdImageRed(img,color);
          green=gdImageGreen(img,color);
          blue=gdImageBlue(img,color);
          tmp=(red+green+blue)/3;
          red=green=blue=tmp;
          #pragma omp critical
          {
            color=gdImageColorAllocate(img,red,green,blue);
            gdImageSetPixel(img,x,y,color);
          }
        }
      }
      t=omp_get_wtime()-t;
      fp1=fopen(oname,"w");
      gdImagePng(img,fp1);
      fclose(fp1);
      gdImageDestroy(img);
      printf("%g\t",t);
    }
    printf("\n");
  }
  return 0;
}
