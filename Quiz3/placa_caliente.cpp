#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

using namespace std;

const int DEFAULT_SIZE = 256;
const double TOLERANCIA = 0.1;

vector<vector<double>> inicializarPlaca(int size) {
    return vector<vector<double>>(size, vector<double>(size, 0.0));
}

int placaCalienteSerial(vector<vector<double>> &placa) {
    int size = placa.size();
    int hotCells = 0;
    bool cambio = true;

    while (cambio) {
        cambio = false;
        vector<vector<double>> nuevaPlaca = placa;

        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                double nuevoValor = ((placa[i+1][j] + placa[i-1][j] + placa[i][j+1] + placa[i][j-1]) + 4 * placa[i][j]) / 8;
                if (fabs(nuevoValor - placa[i][j]) > TOLERANCIA) {
                    cambio = true;
                    nuevaPlaca[i][j] = nuevoValor;
                }
            }
        }

        placa = nuevaPlaca;
    }

    for (int i = 1; i < size - 1; i++)
        for (int j = 1; j < size - 1; j++)
            if (placa[i][j] > 0) hotCells++;

    return hotCells;
}

int placaCalienteParalela(vector<vector<double>> &placa) {
    int size = placa.size();
    int hotCells = 0;
    bool cambio = true;

    while (cambio) {
        cambio = false;
        vector<vector<double>> nuevaPlaca = placa;

        #pragma omp parallel for collapse(2) shared(cambio)
        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                double nuevoValor = ((placa[i+1][j] + placa[i-1][j] + placa[i][j+1] + placa[i][j-1]) + 4 * placa[i][j]) / 8;
                if (fabs(nuevoValor - placa[i][j]) > TOLERANCIA) {
                    #pragma omp atomic write
                    cambio = true;
                    nuevaPlaca[i][j] = nuevoValor;
                }
            }
        }

        placa = nuevaPlaca;
    }

    #pragma omp parallel for reduction(+:hotCells)
    for (int i = 1; i < size - 1; i++)
        for (int j = 1; j < size - 1; j++)
            if (placa[i][j] > 0) hotCells++;

    return hotCells;
}

int main(int argc, char *argv[]) {
    int size = DEFAULT_SIZE;

    if (argc == 2) size = atoi(argv[1]);

    vector<vector<double>> placa = inicializarPlaca(size);

    int hotSerial = placaCalienteSerial(placa);
    cout << "Celdas calientes (Serial): " << hotSerial << endl;

    placa = inicializarPlaca(size);

    int hotParalelo = placaCalienteParalela(placa);
    cout << "Celdas calientes (Paralela): " << hotParalelo << endl;

    return 0;
}
