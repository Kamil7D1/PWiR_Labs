#include <iostream>
#include <mpi.h>


using namespace std;

int max(const int &a, const int &b) {
	return (a < b) ? b : a;
}

int main()
{
	MPI_Init(NULL, NULL);
	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	int n,nRank;
	int *tab;
	if(rank==0)
	{
		cout<<"Podaj N: ";
		cin>>n;
		
		tab=new int[n];
		for(int i=0;i<n;i++)
			tab[i]=i;
			
		for(int i=0;i<n;i++)
			cout<<tab[i]<<" ";
		cout<<endl;
		
		nRank=n / size;
		
		
		int offset = nRank;
		for(int i=1;i<size;i++)
		{
			MPI_Send(&nRank, 1, MPI_INT, i, 88, MPI_COMM_WORLD);
			MPI_Send(&tab[offset],nRank, MPI_INT, i, 88, MPI_COMM_WORLD);
			offset+=nRank;
		}
		
	}
	else
	{
		MPI_Recv(&nRank, 1, MPI_INT, 0, 88, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		tab = new int[nRank];
		MPI_Recv(&tab[0],nRank, MPI_INT, 0, 88, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//cout<<rank<<" "<<nRank<<endl;	

		cout << "Jestem procesem " << rank << " i to jest moja tablica " << endl;
        for(int i = 0; i < nRank; i++){
            cout << tab[i] << " ";
        }
        cout << endl;
	}
	
	int maxRank = tab[0];
	for(int i=1;i<nRank;i++)
	{
		maxRank=max(maxRank, tab[i]);
	}
	
	int maxALL;
	MPI_Reduce(&maxRank,&maxALL,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
	
	if(rank==0)
	{
		cout << "MAX ALL: " << maxALL << endl;
	}
	
	delete [] tab;
	MPI_Finalize();
	return 0;
}
