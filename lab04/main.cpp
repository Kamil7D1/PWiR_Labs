#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>

using namespace std;

int main (int argc, char *argv[])
{ 
	int nRank, rank, size;
	int vectorSize;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// deklaracja wskaźników
	double *A;
	double *B;
	double *C;
	
	// jesli identyfikator procesu to 0
	if(rank == 0){
		// podanie wielkosci tablic
		cout << "rozmiar wektorow: " << endl;
		cin >> vectorSize;
		
		// rezerwacja pamieci dla tablic
		A = new double [vectorSize];
		B = new double [vectorSize];
		C = new double [vectorSize];
		
		// wypelnienie tablicy A i B
		for (int i = 0; i < vectorSize; i++){
			A[i] = i;
			B[i] = i;
		}
		
		// wyswietlenie zawartosci tablicy A
		cout << "A" << endl;
		for(int i = 0; i < vectorSize; i++){
			cout << A[i] << " ";
		}
		
		// wyswietlenie zawartosci tablicy B
		cout << "\n" <<"B" << endl;
		for(int i = 0; i < vectorSize; i++){
			cout << B[i] << " ";
		}

		// wyznaczenie wielkosci tablicy dla kazdego procesu
		nRank = vectorSize/size;
	}
	
	/*
	Funkcja (preces zerowy) rozsyła do wszystkich procesów (do samego siebie też) nRank
	nRank - to co wysylamy
	1 - ile wysylamy
	MPI_INT - typ wysylanej wiadomosci
	0 - nadawca czyli proces 0 w tym przypadku
	komunikator
	*/
	MPI_Bcast(&nRank, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	// rezerwujemy nowe tablice
	double *rankA = new double[nRank];
	double *rankB = new double[nRank];
	double *rankC = new double[nRank];
	
	// Funkcja rozsyla z jednego procesu (nadawcy) do grupy procesow
	/*
	Proces zerowy rozsyła kawalki tablicy A i B do procesów i do siebie samego też.
	Mamy np. [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15];
	O to co dostaja poszczegolne procesy:
	Proces 0 A[0, 1, 2, 3], B[0, 1, 2, 3]
	Proces 1 A[4, 5, 6, 7], B[4, 5, 6, 7]
	Proces 2 A[8, 9, 10, 11], B[8, 9, 10, 11]
	Proces 3 A[12, 13,4 14, 15], B[12, 13,4 14, 15]
	Po czetery elementy po w tym przypadku vectorSize = 16, a size = 4. 16/4 = 4 czyli nRank = 4
	*/	
	MPI_Scatter(A, nRank, MPI_DOUBLE, rankA, nRank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, nRank, MPI_DOUBLE, rankB, nRank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// Każdy proces teraz liczy swoja sume wektorow
    //Proces 0 C[0, 2, 4, 6]
	//Proces 1 C[8, 10, 12, 14]
	//Proces 2 C[16, 18, 20, 22]
	//Proces 3 C[24, 26, 28, 30]

	for(int i = 0; i < nRank; i++){
			rankC[i] = rankA[i] + rankB[i];
	}
	
	// Funkcja zbiera teraz od wszystkich procesow rankC i wysyla do procesu 0
	MPI_Gather(rankC, nRank, MPI_DOUBLE, C, nRank, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// wyswietlenie koncowego wyniku przez proces 0
	if(rank == 0){
        	cout << "\n" << "vector C" << endl;
		for(int i = 0; i < vectorSize; i++){
			cout<< C[i] << " ";
		}
		
		delete[] A;
		delete[] B;
		delete[] C;
	}
	
	delete[] rankA;
	delete[] rankB;
	delete[] rankC;

		
	MPI_Finalize();
	return 0;
}