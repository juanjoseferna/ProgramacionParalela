#include <iostream>
#include <cstdlib>
#include <cmath>
#include <pthread.h>

using namespace std;

int total_tosses, inside_circle = 0;
pthread_mutex_t lock;

long get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void* monte_carlo_thread(void* arg) {
    int tosses_per_thread = total_tosses / *((int*) arg);
    int local_count = 0;
    for (int i = 0; i < tosses_per_thread; i++) {
        double x = (double) rand() / RAND_MAX * 2 - 1;
        double y = (double) rand() / RAND_MAX * 2 - 1;
        if (x * x + y * y <= 1) {
            local_count++;
        }
    }
    pthread_mutex_lock(&lock);
    inside_circle += local_count;
    pthread_mutex_unlock(&lock);
    return nullptr;
}

int main(int argc, char* argv[]) {
    long start_time = get_time_ms();
    if (argc != 3) {
        cout << "ERROR\n";
        return 1;
    }
    int num_threads = atoi(argv[1]);
    total_tosses = atoi(argv[2]);
    pthread_t threads[num_threads];
    int thread_args[num_threads];
    pthread_mutex_init(&lock, nullptr);
    for (int i = 0; i < num_threads; i++) {
        thread_args[i] = num_threads;
        pthread_create(&threads[i], nullptr, monte_carlo_thread, &thread_args[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }
    pthread_mutex_destroy(&lock);
    double pi = 4.0 * inside_circle / total_tosses;
    cout << "Aproximación de Pi: " << pi << ", con " << num_threads << " hilos" <<endl;
    long end_time = get_time_ms();
    long elapsed_ms = end_time - start_time;
    cout << "Tiempo en ejecucion: " << elapsed_ms << " milisegundos\n";
    return 0;
}