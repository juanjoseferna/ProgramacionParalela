#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

int resultA=0, resultB=0, resultC=0, resultD=0, resultF=0, resultE=0;

int sumRange(int start, int end) {
    int s = 0;
    for(int i=start; i<=end; i++){
        s += i;
    }
    return s;
}

int main() {
    double total_start = get_time();

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task depend(out: resultA)
            {
                double start = get_time();
                printf("A inicia\n");
                sleep(2);
                resultA = sumRange(1,5);
                double end = get_time();
                printf("A termina, resultA=%d, time=%.2f s\n", 
                       resultA, end - start);
            }

            #pragma omp task depend(out: resultB)
            {
                double start = get_time();
                printf("B inicia\n");
                sleep(3);
                resultB = sumRange(1,10);
                double end = get_time();
                printf("B termina, resultB=%d, time=%.2f s\n", 
                       resultB, end - start);
            }

            #pragma omp task depend(out: resultC)
            {
                double start = get_time();
                printf("C inicia\n");
                sleep(1);
                resultC = sumRange(1,15);
                double end = get_time();
                printf("C termina, resultC=%d, time=%.2f s\n", 
                       resultC, end - start);
            }

            #pragma omp task depend(in: resultB, resultC) depend(out: resultD)
            {
                double start = get_time();
                printf("D inicia (depende de B, C)\n");
                sleep(1);
                resultD = resultB + resultC;
                double end = get_time();
                printf("D termina, resultD=%d, time=%.2f s\n", 
                       resultD, end - start);
            }

            #pragma omp task depend(in: resultA, resultB) depend(out: resultF)
            {
                double start = get_time();
                printf("F inicia (depende de A, B)\n");
                sleep(1);
                resultF = resultA + resultB;
                double end = get_time();
                printf("F termina, resultF=%d, time=%.2f s\n", 
                       resultF, end - start);
            }

            #pragma omp task depend(in: resultF, resultD) depend(out: resultE)
            {
                double start = get_time();
                printf("E inicia (depende de F, D)\n");
                sleep(1);
                resultE = resultF + resultD;
                double end = get_time();
                printf("E termina, resultE=%d, time=%.2f s\n", 
                       resultE, end - start);
            }
        }
    }

    double total_end = get_time();
    double total_elapsed = total_end - total_start;

    printf("\n[MAIN] Resultados:\n");
    printf("A=%d, B=%d, C=%d, D=%d, F=%d, E=%d\n",
           resultA, resultB, resultC, resultD, resultF, resultE);
    printf("[MAIN] Tiempo TOTAL (OpenMP): %.2f s\n", total_elapsed);

    return 0;
}