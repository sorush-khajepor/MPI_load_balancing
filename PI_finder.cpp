/*  Testing loadBalancer function to calculate PI number with 
    parallel processing techniques (MPI)
	Copyright (C) <2018>  Sorush Khajepor  
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


#include "loadBalancer.h"
#include <math.h>

int main (){

	// Number of terms for calculation of PI
	int N = 1000;

	// MPI initialization 
	int rank;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Begining and end (last+1) index of the chunk for the current processor. 
	int begin; 
	int end;

	// Calling load balancer to find chunk's limits 
	loadBalancer(N,begin,end);
	
	// Calculating a chunk of PI on each processor 
	double PI_chunk = 0.0;
	for (int i=begin;i<end;i++){

	PI_chunk += (4.0/N) * 1.0 / (1.0+ pow((i+0.5)/N,2.0));

	}

	cout<< "rank = "<< rank<<"  my share is = "<< PI_chunk <<endl;

	MPI_Finalize();

}

