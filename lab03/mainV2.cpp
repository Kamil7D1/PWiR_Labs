#include <iostream>
#include <mpi.h>

using namespace std;

int max(const int &a, const int &b) {
    return (a > b) ? a : b;
}

int main(int argv, char *argc[]){
    MPI_Init(NULL, NULL);

    int size, rank, nRank, n;
    int * tab;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        cout << "Podaj liczbe elementow tablicy:" << endl;
        cin >> n;

        tab = new int[n];

        for(int i = 0; i < n; i++){
            tab[i] = i;
        }

        nRank = n / size;
        int offset = nRank;

        for(int i = 1; i < size; i++){
            MPI_Send(&nRank, 1, MPI_INT, i, 102, MPI_COMM_WORLD);
            MPI_Send(&tab[offset], nRank, MPI_INT, i, 102, MPI_COMM_WORLD);
            offset = offset + nRank;
        }

        int mx = tab[0];
        for(int i = 1; i < nRank; i++){
            mx = max(mx, tab[i]);
        }
        cout << "Jestem procesem " << rank << " i to jest moj max " << mx <<endl;

    } else {
        MPI_Recv(&nRank, 1, MPI_INT, 0, 102, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        tab = new int[nRank];
        MPI_Recv(&tab[0], nRank, MPI_INT, 0, 102, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int mx = tab[0];
        for(int i = 1; i < nRank; i++){
            mx = max(mx, tab[i]);
        }

        cout << "Jestem procesem " << rank << " i to jest moj max " << mx <<endl;
    }

    int localMx = tab[0];
    for (int i = 1; i < nRank; i++) {
        localMx = max(localMx, tab[i]);
    }

    int globalMx = 0;
    MPI_Reduce(&localMx, &globalMx,  1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        cout << "Wartosc maksymalna dla wszystkich procesow wynosi " << globalMx << endl;
        delete[] tab; // Free memory only in rank 0
    }

	MPI_Finalize(); 

    return 0;
}