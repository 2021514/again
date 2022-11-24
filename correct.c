#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <math.h>
#include <sched.h>
float fifo_time[10];
float RR_time[10];
float Other_time[10];
int i=0, j=0, k=0;
void countA(){
    for(long long int i=1; i<429496729L; i+=1){
        continue;
    }
    //printf("count worked\n");
}
void countB(){
    for(long long int i=1; i<429496729L; i+=1){
        continue;
    }
    //printf("count worked\n");
}
void countC(){
    for(long long int i=1; i<429496729L; i+=1){
        continue;
    }
    //printf("count worked\n");
}
void* thread_A(void* args){
    struct sched_param parameters;
    int*y = (int*)args;
    parameters.sched_priority = *y;
    int ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &parameters);
    //printf("%d\n", ret);
    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start );
    countA();
    clock_gettime( CLOCK_REALTIME, &stop );
    //time_RR[i] = ((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L));
    double time = (double)(stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1000000000L;
    fifo_time[i] = time;
    i++;
    printf( "SCHED_FIFO for priority[%d]--> %lf\n", parameters.sched_priority, time);

    return NULL;
}
void* thread_B(void* args){
    struct sched_param parameters;
    int*y = (int*)args;
    parameters.sched_priority = *y;
    int ret = pthread_setschedparam(pthread_self(), SCHED_RR, &parameters);
    //printf("%d\n", ret);
    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start );
    countB();
    clock_gettime( CLOCK_REALTIME, &stop );
    //time_RR[i] = ((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L));
    double time = (double)(stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1000000000L;
    RR_time[j] = time;
    j++;
    printf( "SCHED_RR for priority[%d]--> %lf\n", parameters.sched_priority,((double)(stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1000000000L));

    return NULL;
}
void* thread_C(void* args){
    struct sched_param parameters;
    int*y = (int*)args;
    parameters.sched_priority = *y;
    int ret = pthread_setschedparam(pthread_self(), SCHED_OTHER, &parameters);
    //printf("%d\n", ret);
    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start );
    //struct sched_param parameters;
    //int*y = (int*)args;
    countC();
    clock_gettime( CLOCK_REALTIME, &stop );
    //time_RR[i] = ((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L));
    double time = (double)(stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1000000000L;
    Other_time[k]=time;
    k++;
    printf( "SCHED_OTHER for priority 0--> %lf\n",( (double)(stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1000000000L));

    return NULL;
}
int main(){
    pthread_t A;
    pthread_t B;
    pthread_t C;
    int prioritiesA[10] = {12,32,37,45,55,60,64,68,71,73};
    int prioritiesB[10] = {9,22,27,65,69,73,84,88,91,98};
    int prioritiesC[10] = {0};
    printf("--------SCHED_FIFO--------\n");
    for (int i=0; i<10; i++){
        int* x = &prioritiesA[i];
        int* y = &prioritiesB[i];
        int* z = &prioritiesC[0];
        pthread_create(&A, NULL, thread_A, (void*)x);
        pthread_create(&B, NULL, thread_B, (void*)y);
        pthread_create(&C, NULL, thread_C, (void*)z);
        pthread_join(A, NULL);
        pthread_join(B, NULL);
        pthread_join(C, NULL);

    }
    // for (int i=0; i<10; i++){
    //     printf("%lf\n",fifo_time[i]);
    //     printf("%lf\n",RR_time[i]);
    //     printf("%lf\n",Other_time[i]);
    // }
    FILE *fp;
    FILE *ptr;
    FILE *ptr1;
    fp = fopen("FIFO.txt", "w");
    ptr = fopen("RR.txt","w");
    ptr1 = fopen("OTHER.txt","w");
    for (int i=0; i<10; i++){
        fprintf(fp,"%d %lf\n",prioritiesA[i],fifo_time[i]);
    }
    printf("\n");
    for (int i=0; i<10; i++){
        fprintf(ptr,"%d %lf\n",prioritiesB[i],RR_time[i]);
    }
    printf("\n");
    for (int i=0; i<10; i++){
        fprintf(ptr1,"%d %lf\n",prioritiesC[i],Other_time[i]);
    }
    printf("------------HISTOGRAM FOR FIFO------------\n");
    for (int i=0; i<10; i++){
        int x = (int)(fifo_time[i]*100);
        printf("[%d]",prioritiesA[i]);
        for (int j=0; j<x-20; j++){
            printf("*");
        }
        printf("%lf\n",fifo_time[i]);
    }
    printf("\n");
    printf("------------HISTOGRAM FOR RR------------\n");
    for (int i=0; i<10; i++){
        int x = (int)(RR_time[i]*100);
        printf("[%d]",prioritiesB[i]);
        for (int j=0; j<x-20; j++){
            printf("*");
        }
        printf("%lf\n",RR_time[i]);
    }
    printf("\n");
    printf("------------HISTOGRAM FOR OTHER--------------\n");
    printf("All for priority 0\n");
    for (int i=0; i<10; i++){
        int x = (int)(RR_time[i]*100);
        //printf("[%d]",prioritiesB[i]);
        for (int j=0; j<x-20; j++){
            printf("*");
        }
        printf("%lf\n",RR_time[i]);
    }
}