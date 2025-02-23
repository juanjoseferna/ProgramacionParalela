#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

int resultA = 0;
int resultB = 0;
int resultC = 0;
int resultD = 0;
int resultF = 0;
int resultE = 0;

void* funcionA(void* arg);
void* funcionB(void* arg);
void* funcionC(void* arg);
void* funcionD(void* arg);
void* funcionF(void* arg);

int main(){
    double total_start = get_time();
    pthread_t tA, tB, tC;
    pthread_create(&tA, NULL, funcionA, NULL);
    pthread_create(&tB, NULL, funcionB, NULL);
    pthread_create(&tC, NULL, funcionC, NULL);
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    pthread_t tD, tF;
    pthread_create(&tD, NULL, funcionD, NULL);
    pthread_create(&tF, NULL, funcionF, NULL);
    pthread_join(tD, NULL);
    pthread_join(tF, NULL);
    {
        double start = get_time();
        printf("E inicia (depende de D y F)\n");
        sleep(1);
        resultE = resultD + resultF;
        double end = get_time();
        double elapsed = end - start;
        printf("E termina, resultE=%d, time=%.2f s\n", resultE, elapsed);
    }
    double total_end = get_time();
    double total_elapsed = total_end - total_start;
    printf("\n[MAIN] Resultados finales:\n");
    printf("A=%d, B=%d, C=%d, D=%d, F=%d, E=%d\n",
           resultA, resultB, resultC, resultD, resultF, resultE);
    printf("[MAIN] Tiempo TOTAL (pthreads): %.2f seg\n", total_elapsed);

    return 0;
}

int sumRange(int start, int end) {
    int s = 0;
    for(int i = start; i <= end; i++) {
        s += i;
    }
    return s;
}

void* funcionA(void* arg) {
    double start = get_time();
    printf("A inicia\n");
    sleep(2);
    resultA = sumRange(1, 5);
    double end = get_time();
    double elapsed = end - start;
    printf("A termina, resultA=%d, time=%.2f s\n", resultA, elapsed);
    return NULL;
}

void* funcionB(void* arg) {
    double start = get_time();
    printf("B inicia\n");
    sleep(3);
    resultB = sumRange(1, 10);
    double end = get_time();
    double elapsed = end - start;
    printf("B termina, resultB=%d, time=%.2f s\n", resultB, elapsed);
    return NULL;
}

void* funcionC(void* arg) {
    double start = get_time();
    printf("C inicia\n");
    sleep(1);
    resultC = sumRange(1, 15);
    double end = get_time();
    double elapsed = end - start;
    printf("C termina, resultC=%d, time=%.2f s\n", resultC, elapsed);
    return NULL;
}

void* funcionD(void* arg) {
    double start = get_time();
    printf("D inicia (depende de B, C)\n");
    sleep(1);
    resultD = resultB + resultC;
    double end = get_time();
    double elapsed = end - start;
    printf("D termina, resultD=%d, time=%.2f s\n", resultD, elapsed);
    return NULL;
}

void* funcionF(void* arg) {
    double start = get_time();
    printf("F inicia (depende de A, B)\n");
    sleep(1);
    resultF = resultA + resultB;
    double end = get_time();
    double elapsed = end - start;
    printf("F termina, resultF=%d, time=%.2f s\n", resultF, elapsed);
    return NULL;
}