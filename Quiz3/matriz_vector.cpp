#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

using namespace std;

vector<vector<int>> generarMatriz(int m, int n) {
    vector<vector<int>> matriz(m, vector<int>(n));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            matriz[i][j] = rand() % 10;
    return matriz;
}

vector<int> generarVector(int n) {
    vector<int> vec(n);
    for (int i = 0; i < n; i++)
        vec[i] = rand() % 10;
    return vec;
}

vector<int> multiplicarSerial(const vector<vector<int>> &matriz, const vector<int> &vec) {
    int m = matriz.size(), n = vec.size();
    vector<int> resultado(m, 0);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            resultado[i] += matriz[i][j] * vec[j];
    return resultado;
}

vector<int> multiplicarParalelo(const vector<vector<int>> &matriz, const vector<int> &vec) {
    int m = matriz.size(), n = vec.size();
    vector<int> resultado(m, 0);
    
    #pragma omp parallel for
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            resultado[i] += matriz[i][j] * vec[j];

    return resultado;
}

int main(int argc, char *argv[]) {
    int m = 3, n = 3;
    if (argc == 3) {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    vector<vector<int>> matriz = generarMatriz(m, n);
    vector<int> vec = generarVector(n);
    vector<int> resultadoSerial = multiplicarSerial(matriz, vec);
    vector<int> resultadoParalelo = multiplicarParalelo(matriz, vec);
    cout << "Resultado Serial:\n";
    for (int x : resultadoSerial) cout << x << " ";
    cout << "\nResultado Paralelo:\n";
    for (int x : resultadoParalelo) cout << x << " ";
    cout << endl;

    return 0;
}
