#include <vector>
#include <random>
#include <stdio.h>
#include <omp.h>
#include "argh.h"

#define timeit(f) ({ double __time0 = omp_get_wtime(); f; omp_get_wtime() - __time0; })

#ifndef SCHEDULE
#define SCHEDULE schedule(static)
#define SCHEDULE_STR "schedule(static)"
#endif

int threads = 1, size = 1e6, repeat = 1;

template<typename T, int min=0, int max=1>
void uniform_fill(std::vector<T>& array) {
  std::uniform_real_distribution<T> distribution(min, max);
  int size = array.size();
  #pragma omp parallel num_threads(threads)
  {
    std::default_random_engine generator; 
    
    #pragma omp for SCHEDULE
    for (int i = 0; i < size; i++) {
      array[i] = distribution(generator);
    }
  }
}

int main(int argc, char* argv[]) {
  argh::parser cmdl(argv);

  cmdl({ "-t", "--threads"}) >> threads;
  cmdl({ "-s", "--size" }) >> size;
  cmdl({ "-r", "--repeat" }) >> repeat;

  std::vector<double> data(size);  

  for (int i = 0; i < repeat; i++) {
    double time = timeit( uniform_fill(data) );
    printf("%i;%i;%s;%lf\n", threads, size, SCHEDULE_STR, time);
  }
}
