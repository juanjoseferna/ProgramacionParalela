#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

using namespace std;

vector<vector<int>> generarMatriz(int n) {
    vector<vector<int>> matriz(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matriz[i][j] = rand() % 10;
    return matriz;
}

vector<vector<int>> sumaMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

vector<vector<int>> multiplicarMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

vector<vector<int>> transponerMatriz(const vector<vector<int>> &A) {
    int n = A.size();
    vector<vector<int>> T(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            T[j][i] = A[i][j];
    return T;
}

void operacionesParalelas(const vector<vector<int>> &A, const vector<vector<int>> &B,
                          vector<vector<int>> &C_suma, vector<vector<int>> &C_mult, vector<vector<int>> &C_transp) {
    int n = A.size();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    C_suma[i][j] = A[i][j] + B[i][j];
        }

        #pragma omp section
        {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    for (int k = 0; k < n; k++)
                        C_mult[i][j] += A[i][k] * B[k][j];
        }

        #pragma omp section
        {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    C_transp[j][i] = A[i][j];
        }
    }
}

int main(int argc, char *argv[]) {
    int n = 3;
    
    if (argc == 2) n = atoi(argv[1]);
    
    vector<vector<int>> A = generarMatriz(n);
    vector<vector<int>> B = generarMatriz(n);
    
    vector<vector<int>> C_suma = sumaMatrices(A, B);
    vector<vector<int>> C_mult = multiplicarMatrices(A, B);
    vector<vector<int>> C_transp = transponerMatriz(A);

    vector<vector<int>> C_suma_par(n, vector<int>(n, 0));
    vector<vector<int>> C_mult_par(n, vector<int>(n, 0));
    vector<vector<int>> C_transp_par(n, vector<int>(n, 0));

    operacionesParalelas(A, B, C_suma_par, C_mult_par, C_transp_par);
    
    cout << "Suma Serial vs Paralela: " << (C_suma == C_suma_par ? "OK" : "ERROR") << endl;
    cout << "Multiplicación Serial vs Paralela: " << (C_mult == C_mult_par ? "OK" : "ERROR") << endl;
    cout << "Transposición Serial vs Paralela: " << (C_transp == C_transp_par ? "OK" : "ERROR") << endl;

    return 0;
}
