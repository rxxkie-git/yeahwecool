#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data[100];
    int recv_data[10];
    int gathered_data[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (100 % size != 0) {
        if (rank == 0)
            printf("Array size (100) must be divisible by number of processes.\n");
        MPI_Finalize();
        return 0;
    }

    int chunk_size = 100 / size;

    if (rank == 0) {
        for (int i = 0; i < 100; i++)
            send_data[i] = i + 1;

        printf("Root process initialized array to scatter.\n");
    }

    MPI_Scatter(send_data, chunk_size, MPI_INT,
                recv_data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received data: ", rank);
    for (int i = 0; i < chunk_size; i++)
        printf("%d ", recv_data[i]);
    printf("\n");

    for (int i = 0; i < chunk_size; i++)
        recv_data[i] *= 2;

    MPI_Gather(recv_data, chunk_size, MPI_INT,
               gathered_data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Root process gathered modified data: ");
        for (int i = 0; i < 100; i++)
            printf("%d ", gathered_data[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}