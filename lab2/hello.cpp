#include <iostream>
#include <random>
#include <omp.h>

const int SIZE = 1000000;
double array[SIZE];

int main() {
  std::cerr << "$env:Threads = " << omp_get_num_threads() << std::endl;

  std::uniform_real_distribution<double> distribution(0, 1);

  double t0 = omp_get_wtime();
  #pragma omp parallel
  {
    std::default_random_engine generator; 
    
    #pragma omp for schedule(static)
    for (int i = 0; i < SIZE; i++) {
      array[i] = distribution(generator);
    }
  }
  double time = omp_get_wtime() - t0;
  std::cout << time << std::endl;
}
