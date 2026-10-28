#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long long fib(int n) {
    long long x, y;

    if (n <= 1)
        return n;

    #pragma omp task shared(x)
    x = fib(n - 1);

    #pragma omp task shared(y)
    y = fib(n - 2);

    #pragma omp taskwait
    return x + y;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n_terms>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    printf("Fibonacci series up to %d terms:\n", n);

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < n; i++) {
                long long result = fib(i);
                printf("Fib(%d) = %lld\n", i, result);
            }
        }
    }

    return 0;
}