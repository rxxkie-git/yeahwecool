#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_iterations>\n", argv[0]);
        return 1;
    }

    int num_iterations = atoi(argv[1]);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        #pragma omp for schedule(static, 2)
        for (int i = 0; i < num_iterations; i += 2) {
            int end = i + 1;
            if (end >= num_iterations)
                end = num_iterations - 1;

            printf("Thread %d: Iterations %d -- %d\n",
                   thread_id, i, end);
        }
    }

    return 0;
}