#include <iostream>
#include <cstdlib>

using namespace std;

double calculate_pi(int upper_bound) {
    double pi = 0.0;
    for (int k = 0; k < upper_bound; k++) {
        pi += (k % 2 == 0 ? 1.0 : -1.0) / (2 * k + 1);
    }
    return 4 * pi;
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
    
    int upper_bound = atoi(argv[1]);
    double pi = calculate_pi(upper_bound);
    cout << "Aproximación de Pi: " << pi << ", con " << upper_bound << " iteracioens" <<endl;
    long end_time = get_time_ms();
    long elapsed_ms = end_time - start_time;
    cout << "Tiempo en ejecucion: " << elapsed_ms << " milisegundos\n";
    return 0;
}