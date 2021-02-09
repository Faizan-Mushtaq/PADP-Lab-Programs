#include<stdio.h>
#include<omp.h>
#include<ctype.h>
#include<string.h>
#define COUNT 10
char search_words[20][COUNT]={"The","around","graphics","from","by","be","a","which","various","mount"};
long counts[COUNT];

void readword(char *temp,FILE *fp){
  int i=0;
  char ch;
  while((ch=fgetc(fp))!=EOF && isalpha(ch)==0){

  }
  while(ch!=EOF && isalpha(ch)!=0){
    temp[i++]=ch;
    ch=fgetc(fp);
  }
  temp[i]='\0';//printf("%s\n",temp);
}
long get_count(const char* filename,const char *key){
  int key_len=strlen(key);
  long word_count=0;
  FILE *fp=fopen(filename,"r");
  char temp[40];
  while(feof(fp)==0){
    readword(temp,fp);
    if(strcasecmp(temp,key)==0)
     word_count++;
  }
  fclose(fp);
  return word_count;
}
int main(){
  int i;
  for(i=0;i<COUNT;i++)
   counts[i]=0;
   char* myfiles[4]={"file1.txt","file2.txt","file3.txt","file4.txt"};
   printf("Size\t\tT1\t\tT2\t\tT4\t\tT8");
   for(int j=0;j<4;j++)
   {
     FILE *fp=fopen(myfiles[j],"r");
     fseek(fp,0L,SEEK_END);
     printf("\n%.2lfKB\t",ftell(fp)/1024.0);
     if(j!=4) printf("\t");
     fclose(fp);
     for(int t=1;t<=8;t*=2)
     {
       omp_set_num_threads(t);
       double t1=omp_get_wtime();
       #pragma omp parallel for
       for(int i=0;i<COUNT;i++)
       {
         counts[i]=get_count(myfiles[j],search_words[i]);
       }
         t1=omp_get_wtime()-t1;
         //			for(i=0;i<COUNT;i++) printf("%s: %ld  ",search_words[i],counts[i]);
         printf("%lf s\t",t1);
     }
   }
   return 0;
}
