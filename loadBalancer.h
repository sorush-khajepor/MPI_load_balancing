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
*/

#ifndef LOAD_BALANCER
#define LOAD_BALANCER


#include "mpi.h"
#include <iostream>
#include <vector>

using namespace std;

// Use this function to divide a non-contiguous load into chunk vectors
// N = Number of items to be distributed
// chunk = a part of the load vector for the processor running this code.
// It contains index of items only. It is assumed the index
// of items starts from 0.
// For example, Load = Load [0:11] has 12 elements and is supposed to 
// be divided among 5 processors. We have chunk_i = chunk of processor i:
// chunk_0 = {0,   1, 2}
// chunk_1 = {3,   4, 5}
// chunk_2 = {6,   7}
// chunk_3 = {8,   9}
// chunk_4 = {10, 11}
// Therefore, processor of rank 0 works on zeroth, first, and second elemets
// of Load. The data type of Load is not of concernt here. We assumed
// all elemets have the same computational weight. Chunk can be filled
// in a non-contiguous way. 
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
// end = end index of the chunk, like vectors, end = index-of-the-last + 1
// Index of the last element = end - 1 
// Size of each chunk = end - begin
// For example, Load = Load [0:11] has 12 elements and is supposed to 
// be divided among 5 processors. Each processor finds the below values
// Processor    begin       end
//   0 =         0          3
//   1 =         3          6
//   2 =         6          8
//   3 =         8          10
//   4 =         10         12
// Therefore, processor of rank 0 works on zeroth, first, and second elemets
// of Load. The data type of Load is not of concernt here. We assumed
// all elemets have the same computational weight. 
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

#endif


