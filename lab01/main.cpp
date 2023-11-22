#include <iostream>
#include <mpi.h>

using namespace std;

int main (int argc, char *argv[])
{
    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &size);

    cout << "Jestem procesem " << rank << " z " << size << endl;
    
    MPI_Finalize();

    return 0;
}