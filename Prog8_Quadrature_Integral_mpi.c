#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

double f(double x){
    double pi = 3.14;
    return (double)(50/(pi*(2500*x*x + 1)));
}

int main(int argc, char **argv){
    double t;
    int p, id;
    int n = 100000;
    int my_n;
    double exact = 0.49963;
    double error;
    double my_a, my_b, my_total = 0.0, total = 0.0, a = 0.0, b = 10.0;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double intvl = b/(double)(p-1);
    if(id == 0){
        printf("Computing the value of integral f(x) = 50/(pi*(2500*x*x + 1))");
        t = MPI_Wtime();
        my_n = n/(p-1);
    }
    MPI_Bcast(&my_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(id == 0){
        int i;
        for(i=1;i<=p-1;i++){
            my_a = i*intvl - intvl;
            my_b = i*intvl;
            printf("%d %f %f\n",i, my_a,my_b);
            MPI_Send(&my_a, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&my_b, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
    }
    if(id>0){
        MPI_Status status;
        int i;
        double x;
        printf("%d %d\n",id,my_n);
        MPI_Recv(&my_a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&my_b, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        for(i=0;i<my_n;i++){
            x = ((my_n - i)*my_a + (i-1)*my_b)/(double)(my_n-1);
            my_total+=f(x);
        }
        my_total = (my_b-my_a)*my_total/(double)(my_n);
    }
    MPI_Reduce(&my_total, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(id == 0){
        error = exact - total;
        printf("Integral computed is %f with error %f in time %f",total,error,MPI_Wtime()-t);
    }
    MPI_Finalize();
}
