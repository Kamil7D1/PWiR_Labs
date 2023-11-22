#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]){
    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int msg = 0;
 
        if(rank == 0){
            MPI_Send(&msg, 1, MPI_INT, 1, 1,MPI_COMM_WORLD);
        }

        if(rank < size - 1){
            MPI_Recv(&msg, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            msg = msg + rank;
            MPI_Send(&msg, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        }

        if(rank == size - 1){
            MPI_Recv(&msg, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            msg = msg + rank;
            cout << "Jestem procesem " << rank << " i o to wynik: " << msg << endl;
        }

    MPI_Finalize();
    return 0;
} 