#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <math.h>
#include <sched.h>
void countA(){
    for(long long int i=1; i<429496729L; i+=1){
        continue;
    }
}
void countB(){
    for(long long int i=1; i<429496729L; i+=1){
        continue;
    }
}
void countC(){
    for(long long int i=1; i<429496729L; i+=1){
        continue;
    }
}
void* thread_A(void* args){
    struct sched_param parameters;
    int*y = (int*)args;
    parameters.sched_priority = *y;
    int ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &parameters);
    printf("%d\n", ret);
    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start );
    countA();
    clock_gettime( CLOCK_REALTIME, &stop );
    //time_RR[i] = ((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L));
    printf( "SCHED_FIFO for priority[%d]--> %lf\n", parameters.sched_priority,((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L)) );

    return NULL;
}
void* thread_B(void* args){
    struct sched_param parameters;
    int*y = (int*)args;
    parameters.sched_priority = *y;
    int ret = pthread_setschedparam(pthread_self(), SCHED_RR, &parameters);
    printf("%d\n", ret);
    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start );
    countB();
    clock_gettime( CLOCK_REALTIME, &stop );
    //time_RR[i] = ((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L));
    printf( "SCHED_RR for priority[%d]--> %lf\n", parameters.sched_priority,((double)(stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1000000000L));

    return NULL;
}
void* thread_C(void* args){struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start );
    struct sched_param parameters;
    int*y = (int*)args;
    countC();
    clock_gettime( CLOCK_REALTIME, &stop );
    //time_RR[i] = ((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L));
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
        // struct timespec start, stop;
        // clock_gettime( CLOCK_REALTIME,i&start );
        // countA();
        // clock_gettime( CLOCK_REALTIME, &stop );
        // printf( "SCHED_FIFO for priority[%d]--> %lf\n", parameters.sched_priority,((double) (stop.tv_sec - start.tv_sec) + (double) ((stop.tv_nsec - start.tv_nsec)/(double) 1000000000L)) );
        
        pthread_create(&B, NULL, thread_B, (void*)y);
        // struct timespec start1, stop1;
        // clock_gettime( CLOCK_REALTIME, &start1 );
        // countB();
        // clock_gettime( CLOCK_REALTIME, &stop1 );
        // printf( "SCHED_RR for priority[%d]--> %lf\n", parameters.sched_priority,((double) (stop1.tv_sec - start1.tv_sec) + (double) ((stop1.tv_nsec - start1.tv_nsec)/(double) 1000000000L)) );

        pthread_create(&C, NULL, thread_C, (void*)z);
        // struct timespec start2, stop2;
        // clock_gettime( CLOCK_REALTIME, &start2 );
        // countC();
        // clock_gettime( CLOCK_REALTIME, &stop2 );
        // printf( "SCHED_OTHER for priority[%d]--> %lf\n", parameters.sched_priority,((double) (stop2.tv_sec - start2.tv_sec) + (double) ((stop2.tv_nsec - start2.tv_nsec)/(double) 1000000000L)) );
        pthread_join(A, NULL);
        pthread_join(B, NULL);
        pthread_join(C, NULL);

    }
    
    //printf("--------SCHED_RR--------\n");
    // for (int i=0; i<10; i++){
    //     
    //     pthread_create(&B, NULL, thread_B, (void*)y);
    //     pthread_join(B, NULL);
    // }
    //printf("--------SCHED_OTHER--------\n");
    // for (int i=0; i<10; i++){
    //     int* z = &prioritiesC[i];
    //     pthread_create(&C, NULL, thread_C, (void*)z);
    //     pthread_join(C, NULL);
    // }
}