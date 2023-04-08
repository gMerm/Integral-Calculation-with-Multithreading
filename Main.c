#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

double a=1.0, b=4.0;
int n=10000000;
int num_threads;

double get_wtime(void)                                                      
{
    struct timeval t;

    gettimeofday(&t, NULL);

    return (double)t.tv_sec + (double)t.tv_usec*1.0e-6;
}

double f(double x){
    return log(x)*sqrt(x);
}

void *calculate_integral(void *thread_id){

    int id = *(int *)thread_id;
    int chunk_size=n/num_threads;
    int start=id*chunk_size;
    int end=(id + 1)*chunk_size;
    double dx=(b-a)/n;
    double sum=0.0;

    for (int i=start; i<end; i++){
        double x=a+(i+0.5)*dx;
        sum+=f(x);
    }
    double *result=malloc(sizeof(double));
    *result=sum*dx;
    pthread_exit(result);
}

int main() {

    system("clear");
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    double t0=get_wtime();

    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    for (int i=0; i<num_threads; i++){
        thread_ids[i]=i;
        pthread_create(&threads[i], NULL, calculate_integral, &thread_ids[i]);
    }

    double total=0.0;
    for (int i=0; i<num_threads; i++){
        double *result;
        pthread_join(threads[i], (void **)&result);
        total+=*result;
        free(result);
    }

    double t1=get_wtime();
    double timeTotal=t1-t0;

    printf("The integral is approximately: %lf and took: %lf to calculate.\n", total, timeTotal);

    return 0;
}
