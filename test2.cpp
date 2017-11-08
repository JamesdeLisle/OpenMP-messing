#include <stdio.h>
#include <iostream>
#include <time.h>
#include "omp.h"
#include <fstream>
#include <cstdlib>
#include <cmath>

static long num = 1000;
using namespace std;

int pm(int val) {
    val++;
    val--;
    return val;
}

int main() {
    
    int i, j, k, l;
    int iter = 8;
    int iterT = 4;
    long inum[iter];
    for (i=0; i<iter; i++) inum[i] = pow(10, i); 
    
    double serial[iter][iterT], parallel[iter][iterT];
    
    ofstream outdata;
    outdata.open("output.dat");
    if (!outdata) {
        std::cerr << "Could not open file." << std::endl;
        exit(1);
    }
    
    for (l=1; l<iterT+1; l++) {
        for (k=0; k<iter; k++) {
            clock_t start = clock();
            long int A[num], B[num];
            omp_set_num_threads(l);
            for (i=0; i<num; i++) A[i] = i;
            for (i=0; i<num; i++){
                #pragma omp parallel for schedule(static)
                for (j=0; j<inum[k]; j++) {
                    B[i] = pm(A[i]);
                }
                std::cout << "\r" << "Parallel: " << l << " - " << 100.0 * i / num;
            } 
            std::cout << std::endl;
            clock_t finish = clock();
            parallel[k][l-1] = (double) (finish - start) / CLOCKS_PER_SEC * 1000.0;
            
            start = clock();
            long int C[num], D[num];
            for (i=0; i<num; i++) C[i] = i;
            for (i=0; i<num; i++){
                for (j=0; j<inum[k]; j++) {
                    D[i] = pm(C[i]);
                }
                std::cout << "\r" << "Series: " << l << " - " << 100.0 * i / num;
            }
            std::cout << std::endl;
            finish = clock();
            serial[k][l-1] = (double) (finish - start) / CLOCKS_PER_SEC * 1000.0;
        }
    }

    for (j=0; j<iterT; j++) {
        for (i=0; i<iter; i++) {
            outdata << inum[i] << " " << j + 1 << " " << serial[i][j] << " " << parallel[i][j]<< std::endl;
        }
    }
    outdata.close();
    return 0;
}
