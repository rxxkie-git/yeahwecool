#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void sequential_merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sequential_merge_sort(arr, l, m);
        sequential_merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallel_merge_sort(int arr[], int l, int r, int depth) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth <= 0) {
            sequential_merge_sort(arr, l, m);
            sequential_merge_sort(arr, m + 1, r);
        } else {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallel_merge_sort(arr, l, m, depth - 1);

                #pragma omp section
                parallel_merge_sort(arr, m + 1, r, depth - 1);
            }
        }
        merge(arr, l, m, r);
    }
}

void copy_array(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

int main() {
    int n = 100000;
    int *arr = (int *)malloc(n * sizeof(int));
    int *arr_seq = (int *)malloc(n * sizeof(int));
    int *arr_par = (int *)malloc(n * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100000;

    copy_array(arr, arr_seq, n);
    copy_array(arr, arr_par, n);

    double start_seq = omp_get_wtime();
    sequential_merge_sort(arr_seq, 0, n - 1);
    double end_seq = omp_get_wtime();

    double start_par = omp_get_wtime();
    parallel_merge_sort(arr_par, 0, n - 1, 4);
    double end_par = omp_get_wtime();

    printf("Sequential Merge Sort Time: %f seconds\n",
           end_seq - start_seq);
    printf("Parallel Merge Sort Time : %f seconds\n",
           end_par - start_par);

    free(arr);
    free(arr_seq);
    free(arr_par);
    return 0;
}