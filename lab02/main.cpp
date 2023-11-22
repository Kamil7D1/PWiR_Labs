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
        MPI_Send(&msg, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        // (wiadomosc, ile, typ_danych, do_kogo?, tag, komunikator)
    }
    if(rank == 1){
        // int b;
        // MPI_Recv(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&msg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // (wiadomosc, ile, typ, od_kogo?, tag, komunikator, status)
        // wiadomosc nie musi byc taka sama jak wczesniej msg moze byc np. &b
        msg = msg + rank;
        MPI_Send(&msg, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
    }
    if(rank == 2){
        MPI_Recv(&msg, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        msg = msg + rank;
        MPI_Send(&msg, 1, MPI_INT, 3, 3, MPI_COMM_WORLD);
    }
    if(rank == 3){
        MPI_Recv(&msg, 1, MPI_INT, 2, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        msg = msg + rank;
        cout << "Jestem procesem " << rank << " i o to wynik: " << msg << endl;
    }

    MPI_Finalize();
    return 0;
}