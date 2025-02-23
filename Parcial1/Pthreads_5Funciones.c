#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int resultA = 0, resultB = 0, resultC = 0, resultD = 0, resultE = 0;

int sumRange(int start, int end) {
    int s = 0;
    for (int i = start; i <= end; i++) {
        s += i;
    }
    return s;
}

void* funcionA(void* arg);
void* funcionB(void* arg);
void* funcionC(void* arg);

int main() {
    pthread_t threadA, threadB, threadC;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    pthread_create(&threadA, NULL, funcionA, NULL);
    pthread_create(&threadB, NULL, funcionB, NULL);
    pthread_create(&threadC, NULL, funcionC, NULL);
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    resultD = resultB + resultC;
    resultE = resultA + resultD;

    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) 
                   + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("[MAIN] resultA=%d, resultB=%d, resultC=%d, resultD=%d, resultE=%d\n",
           resultA, resultB, resultC, resultD, resultE);

    printf("[MAIN] Tiempo total: %.2f segundos\n", elapsed);
    return 0;
}

void* funcionA(void* arg) {
    printf("funcionA: iniciada\n");
    sleep(2);
    resultA = sumRange(1, 5);
    printf("funcionA: finalizada (resultA=%d)\n", resultA);
    return NULL;
}

void* funcionB(void* arg) {
    printf("funcionB: iniciada\n");
    sleep(3);
    resultB = sumRange(1, 10);
    printf("funcionB: finalizada (resultB=%d)\n", resultB);
    return NULL;
}

void* funcionC(void* arg) {
    printf("funcionC: iniciada\n");
    sleep(1);
    resultC = sumRange(1, 15);
    printf("funcionC: finalizada (resultC=%d)\n", resultC);
    return NULL;
}