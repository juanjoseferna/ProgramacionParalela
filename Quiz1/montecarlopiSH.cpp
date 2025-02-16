#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

double monte_carlo_pi(int tosses) {
    int inside_circle = 0;
    for (int i = 0; i < tosses; i++) {
        double x = (double) rand() / RAND_MAX * 2 - 1;
        double y = (double) rand() / RAND_MAX * 2 - 1;
        if (x * x + y * y <= 1) {
            inside_circle++;
        }
    }
    return 4.0 * inside_circle / tosses;
}

long get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int main(int argc, char *argv[]) {
    long start_time = get_time_ms();
    if (argc != 2) {
        cout << "ERROR\n";
        return 1;
    }

    int tosses = atoi(argv[1]);
    double pi = monte_carlo_pi(tosses);
    cout << "Aproximación de Pi: " << pi << ", con " << tosses << " iteracioens" <<endl;
    long end_time = get_time_ms();
    long elapsed_ms = end_time - start_time;
    cout << "Tiempo en ejecucion: " << elapsed_ms << " milisegundos\n";
    return 0;
}