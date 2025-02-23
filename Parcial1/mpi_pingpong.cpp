#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    printf("Proceso %d de %d\n", world_rank, world_size);
    if (world_size != 1) {
        if (world_rank == 0) {
            std::cerr << "Este programa requiere exactamente 2 procesos.\n";
        }
        MPI_Finalize();
        return 1;
    }
    const int NUM_EXCHANGES = 10;
    std::vector<double> rtt_values(NUM_EXCHANGES, 0.0);
    std::srand(std::time(nullptr));

    if (world_rank == 0) {
        for (int i = 0; i < NUM_EXCHANGES; i++) {
            int mensaje = std::rand() % 100;
            double start = MPI_Wtime();
            MPI_Send(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double end = MPI_Wtime();
            rtt_values[i] = (end - start);
            std::cout << "Intercambio " << i 
                      << ": Mensaje recibido=" << mensaje 
                      << " (RTT=" << rtt_values[i] << "s)\n";
        }
        double suma = 0.0;
        for (auto val : rtt_values) {
            suma += val;
        }
        std::cout << "RTT promedio tras " << NUM_EXCHANGES 
                  << " intercambios: " << (suma / NUM_EXCHANGES) << " s\n";
    } else {
        for (int i = 0; i < NUM_EXCHANGES; i++) {
            int mensaje;
            MPI_Recv(&mensaje, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            mensaje++;
            MPI_Send(&mensaje, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}