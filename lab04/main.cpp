#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argv,char *argc[]) {
    MPI_Init(NULL, NULL);

    int rank, size, nRank, n;
    double *a;
    double *b;
    double *c;

    if(rank == 0){
        cout << "Podaj n:" << endl;
        cin >> n;

        a = new double[n];
        b = new double[n];

        for(int i = 0; i < n; i++){
            a[i] = i;
            b[i] = i;
        }

        nRank = n / size;
        double dest1[nRank];
        double dest2[nRank];

        MPI_Bcast(&nRank, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&a, nRank, MPI_DOUBLE, dest1, nRank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(&b, nRank, MPI_DOUBLE, dest2, nRank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&nRank, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
        cout << "Proces: " << rank << "nRank" << nRank << endl; 
        
    }

    delete [] a;
    delete [] b;
    MPI_Finalize();

    return 0;
}