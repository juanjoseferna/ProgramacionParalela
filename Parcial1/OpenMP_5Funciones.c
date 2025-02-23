#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int resultA, resultB, resultC, resultD, resultE;

int sumRange(int start, int end) {
    int s = 0;
    for(int i = start; i <= end; i++) {
        s += i;
    }
    return s;
}

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task depend(out: resultA)
            {
                printf("A inicia\n");
                sleep(2); 
                resultA = sumRange(1, 5); 
                printf("A termina, resultA = %d\n", resultA);
            }
            
            #pragma omp task depend(out: resultB)
            {
                printf("B inicia\n");
                sleep(3);
                resultB = sumRange(1, 10);
                printf("B termina, resultB = %d\n", resultB);
            }

            #pragma omp task depend(out: resultC)
            {
                printf("C inicia\n");
                sleep(1);
                resultC = sumRange(1, 15);
                printf("C termina, resultC = %d\n", resultC);
            }
            
            #pragma omp task depend(in: resultB, resultC) depend(out: resultD)
            {
                printf("D inicia (depende de B y C)\n");
                sleep(1);
                resultD = resultB + resultC;
                printf("D termina, resultD = %d\n", resultD);
            }

            #pragma omp task depend(in: resultA, resultD) depend(out: resultE)
            {
                printf("E inicia (depende de A y D)\n");
                sleep(1);
                resultE = resultA + resultD;
                printf("E termina, resultE = %d\n", resultE);
            }
        } 
    }

    printf("[MAIN] resultA=%d, resultB=%d, resultC=%d, resultD=%d, resultE=%d\n",
           resultA, resultB, resultC, resultD, resultE);

    return 0;
}