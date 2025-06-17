#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <chrono>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <algorithm>

using namespace emscripten;
using namespace std::chrono;

#define SIZE 1000000
#define ITER 100000000

// Funções auxiliares para marcação de tempo
double now() {
    return duration<double, std::milli>(high_resolution_clock::now().time_since_epoch()).count();
}

// Benchmarks matemáticos

double int_add_latency() {
    volatile int sum = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITER; i++) sum += i;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double int_mult_latency() {
    volatile int prod = 1;
    auto start = high_resolution_clock::now();
    for (int i = 1; i < ITER; i++) prod *= 1;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double int_div_latency() {
    volatile int res = 1;
    auto start = high_resolution_clock::now();
    for (int i = 1; i < ITER; i++) res /= 1;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double float_add_latency() {
    volatile float sum = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITER; i++) sum += 1.0f;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double float_mult_latency() {
    volatile float prod = 1.0f;
    auto start = high_resolution_clock::now();
    for (int i = 1; i < ITER; i++) prod *= 1.000001f;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double float_div_latency() {
    volatile float res = 1.0f;
    auto start = high_resolution_clock::now();
    for (int i = 1; i < ITER; i++) res /= 1.000001f;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double double_add_latency() {
    volatile double sum = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITER; i++) sum += 1.0;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double double_mult_latency() {
    volatile double prod = 1.0;
    auto start = high_resolution_clock::now();
    for (int i = 1; i < ITER; i++) prod *= 1.000001;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double double_div_latency() {
    volatile double res = 1.0;
    auto start = high_resolution_clock::now();
    for (int i = 1; i < ITER; i++) res /= 1.000001;
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

// Benchmarks de memória

double sequential_memory_latency() {
    std::vector<int> arr(SIZE);
    for (int i = 0; i < SIZE; i++) arr[i] = i;
    volatile int total = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < SIZE; i++) total += arr[i];
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double random_memory_latency() {
    std::vector<int> arr(SIZE);
    for (int i = 0; i < SIZE; i++) arr[i] = i;
    volatile int total = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < SIZE; i++) total += arr[rand() % SIZE];
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double heap_allocation_latency() {
    auto start = high_resolution_clock::now();
    int* large_block = (int*)malloc(100 * 1024 * 1024);
    for (int i = 0; i < 100000; i++) large_block[i] = i;
    auto end = high_resolution_clock::now();
    free(large_block);
    return duration<double, std::milli>(end - start).count();
}

// Benchmarks especiais

double benchmark_simd_vs_scalar() {
    const int size = 1024;
    float a[size], b[size], result[size];

    for (int i = 0; i < size; ++i) {
        a[i] = i * 0.5f;
        b[i] = i * 0.25f;
    }

    auto start = high_resolution_clock::now();
    for (int i = 0; i < size; i += 4) {
        result[i] = a[i] + b[i];
        result[i+1] = a[i+1] + b[i+1];
        result[i+2] = a[i+2] + b[i+2];
        result[i+3] = a[i+3] + b[i+3];
    }
    auto end = high_resolution_clock::now();

    return duration<double, std::milli>(end - start).count();
}

double microbenchmark_nbody() {
    struct Body {
        double x, y, z, vx, vy, vz, mass;
    };

    const int n = 100;
    std::vector<Body> bodies(n, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0});
    auto start = high_resolution_clock::now();

    for (int step = 0; step < 10; ++step) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                double dx = bodies[j].x - bodies[i].x;
                double dy = bodies[j].y - bodies[i].y;
                double dz = bodies[j].z - bodies[i].z;
                double dist = sqrt(dx*dx + dy*dy + dz*dz + 1e-10);
                double force = bodies[j].mass / (dist * dist);
                bodies[i].vx += dx * force;
                bodies[i].vy += dy * force;
                bodies[i].vz += dz * force;
            }
        }
    }

    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double branch_prediction_latency() {
    volatile int sum = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITER; i++) {
        if (i % 2 == 0) {
            sum += i;
        } else {
            sum -= i;
        }
    }
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

double cache_associativity_latency() {
    const int stride = 4096; // 4KB - típico tamanho de página
    const int accesses = 100000;
    std::vector<int> arr(stride * 100, 1);

    volatile int sum = 0;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < accesses; i++) {
        sum += arr[(i * stride) % arr.size()];
    }
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

std::string run_benchmarks() {
    std::string json = "{";

    // Operações matemáticas
    json += "\"int_add_latency_ms\":" + std::to_string(int_add_latency()) + ",";
    json += "\"int_mult_latency_ms\":" + std::to_string(int_mult_latency()) + ",";
    json += "\"int_div_latency_ms\":" + std::to_string(int_div_latency()) + ",";
    json += "\"float_add_latency_ms\":" + std::to_string(float_add_latency()) + ",";
    json += "\"float_mult_latency_ms\":" + std::to_string(float_mult_latency()) + ",";
    json += "\"float_div_latency_ms\":" + std::to_string(float_div_latency()) + ",";
    json += "\"double_add_latency_ms\":" + std::to_string(double_add_latency()) + ",";
    json += "\"double_mult_latency_ms\":" + std::to_string(double_mult_latency()) + ",";
    json += "\"double_div_latency_ms\":" + std::to_string(double_div_latency()) + ",";

    // // Benchmarks de memória
    json += "\"sequential_memory_latency_ms\":" + std::to_string(sequential_memory_latency()) + ",";
    json += "\"random_memory_latency_ms\":" + std::to_string(random_memory_latency()) + ",";
    json += "\"heap_allocation_latency_ms\":" + std::to_string(heap_allocation_latency()) + ",";

    // Benchmarks especiais
    json += "\"simd_vs_scalar_ms\":" + std::to_string(benchmark_simd_vs_scalar()) + ",";
    json += "\"nbody_simulation_ms\":" + std::to_string(microbenchmark_nbody()) + ",";
    json += "\"branch_prediction_latency_ms\":" + std::to_string(branch_prediction_latency()) + ",";
    json += "\"cache_associativity_latency_ms\":" + std::to_string(cache_associativity_latency());

    json += "}";

    return json;
}


// Binding
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("run_benchmarks", &run_benchmarks);
}
