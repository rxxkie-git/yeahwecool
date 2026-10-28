#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;

    int sqrt_num = (int)sqrt((double)num);
    for (int i = 3; i <= sqrt_num; i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int *serial_primes = (int *)malloc(n * sizeof(int));
    int *parallel_primes = (int *)malloc(n * sizeof(int));
    int count_serial = 0, count_parallel = 0;

    double start_serial = omp_get_wtime();
    for (int i = 1; i <= n; i++) {
        if (is_prime(i))
            serial_primes[count_serial++] = i;
    }
    double end_serial = omp_get_wtime();

    double start_parallel = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 1; i <= n; i++) {
        if (is_prime(i)) {
            #pragma omp critical
            {
                parallel_primes[count_parallel++] = i;
            }
        }
    }

    double end_parallel = omp_get_wtime();

    printf("\nSerial Time : %f seconds\n",
           end_serial - start_serial);
    printf("Parallel Time : %f seconds\n",
           end_parallel - start_parallel);

    printf("\nNumber of primes found: %d\n", count_serial);
    printf("First 10 primes (serial): ");
    for (int i = 0; i < count_serial && i < 10; i++)
        printf("%d ", serial_primes[i]);

    printf("\n");

    free(serial_primes);
    free(parallel_primes);
    return 0;
}