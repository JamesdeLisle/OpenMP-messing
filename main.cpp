#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include "omp.h"

#define NUMTHREADS 4
static long num_steps = 10000;
using namespace std;

int main() {
    int i, nthreads;
    double pi, sum[NUMTHREADS];
    double step = 1.0/(double) num_steps;
    omp_set_num_threads(NUMTHREADS);
    #pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;
        for (i=id, sum[id]=0.0; i<num_steps; i=i+nthrds){
            x = (i+0.5)*step;
            sum[id] += 4.0/(1.0+x*x) * step;
        }
    }
    std::cout << sizeof(sum)/sizeof(*sum) << std::endl;
    for (i=0, pi=0.0; i<nthreads; i++) pi += sum[i];
    std::cout << pi << std::endl;
    return 0;
}
