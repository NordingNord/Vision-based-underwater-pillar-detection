#include "benchmarking_test.h"
#include<benchmark/benchmark.h>

static void benchmark_pipeline(benchmark::State& state){
    for(auto _ : state){

        tests test_runner;
        test_runner.test_final_implementation();
    }
}

BENCHMARK(benchmark_pipeline);
BENCHMARK_MAIN();
