/*  MPI load balancer for (non-)contiguous load vector (items have same weight).
    Copyright (C) <2015>  Sorush Khajepor  
    sorush.khajepor@gmail.com
    s.khajepor@hw.ac.uk
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    Sequence of running the code:
    - mpicxx loadBalancer.cpp
    - mpirun -n 5 ./a.out
    Change 5 to your desired number of processors.   
    */


#include "mpi.h"
#include <iostream>
#include <vector>

using namespace std;

// Use this function to divide a non-contiguous load into chunk vectors
// N = Number of items to be distributed
// chunk = a part of the load vector for the processor running this code.
// It contains index of items only. It is assumed the index
// of items starts from 0.
int loadBalancer (int N, vector<int>& chunk){

    // MPI parameters 
    int rank;
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Distributing items
    int remainder = N%size;
    int chunksize = (N+size-rank-1)/size;
    int begin = N/size*rank + min(rank,remainder);

    for (int i = begin; i < begin + chunksize; i++){
        chunk.push_back(i);
    }

    // Error handler
    if (N<1) {
        return 0;
    } else{
        return 1;
    }

}

// Use this function to divide a contiguous load vector to contiguous chunks
// N = Number of items to be distributed
// begin = start index of the chunk
// end = end index of the chunk, like vectors end = index-of-the-lastt + 1
int loadBalancer (int N,int &begin,int &end){

    // MPI parameters 
    int rank;
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Distributing items
    int remainder = N%size;
    int chunksize = (N+size-rank-1)/size;
    begin = N/size*rank + min(rank,remainder);
    end = begin + chunksize;
    
    // Error handler
    if (N<1) {
        return 0;
    } else{
        return 1;
    }

}


// Testing functions
int main (){
// Load vector is defined based on the need of the 
// programmer which is not necessary here.

// Number of items in the load vector to be distributed
int N = 101;

// The chunk for the processor running this code.
// It contains only index of items.
vector<int> chunk;

// Begining and end (last+1) index of the chunk for the current processor. 
int begin; 
int end;

// MPI initialization 
int rank;
MPI_Init(NULL,NULL);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

// Calling load balancer by filling chunk vector
loadBalancer(N, chunk);

// Calling load balancer to find chunk's limits 
loadBalancer(N,begin,end);

// Report of load distribution
if (rank==0){
cout << "Number of items = " << N << endl;
cout << "load index starts from 0 to "<< N-1<<endl;
}
// Report of the first function call
cout << "rank = "<<rank<< "  index begins at = "<< chunk.front()
     << "  finishes at = " << chunk.back()
     << "  with chunk size = " << chunk.size()<< endl;
// Report of the second function calll
cout << "rank = "<<rank<< "  index begins at = "<< begin
     << "  finishes at = " << end-1 << endl;


MPI_Finalize();

}
