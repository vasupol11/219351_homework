#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000


void swap(int *xp, int *yp) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n) {
  int i, j;
  for (i = 0; i < n-1; i++)      
    for (j = 0; j < n-i-1; j++)
      if (arr[j] > arr[j+1])
	swap(&arr[j], &arr[j+1]);
}

int isSorted(int *a, int size) {
  int i;
  for (i = 0; i < size - 1; i++) {
    if (a[i] > a[i + 1]) {
      return 0;
    }
  }
  return 1;
}
//A function to implement merge sort
void mergesort (long num,int *a, int *b)
{
    int rght, wid, rend;
    int i,j,m,t;

    for (int k=1; k < num; k *= 2 ) {       
        for (int left=0; left+k < num; left += k*2 ) {
            rght = left + k;        
            rend = rght + k;
            if (rend > num) rend = num; 
            m = left; i = left; j = rght; 
            while (i < rght && j < rend) { 
                if (a[i] <= a[j]) {         
                    b[m] = a[i]; i++;
                } else {
                    b[m] = a[j]; j++;
                }
                m++;
            }
            while (i < rght) { 
                b[m]=a[i]; 
                i++; m++;
            }
            while (j < rend) { 
                b[m]=a[j]; 
                j++; m++;
            }
            for (m=left; m < rend; m++) { 
                a[m] = b[m]; 
            }
        }
    }
}

// Function to print an array
void printArray(int arr[], int size)
{
  int i;
  for (i=0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

int main(int argc, char** argv) {
	int i, n;
	int* A, *temp, *temp2;
	clock_t start, end;
	double elapsed_time, t1, t2;

	MPI_Init(NULL, NULL);
// Find out rank, size
	  int world_rank;
	  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	  int world_size;
	  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	int elements_per_proc = N/world_size;

	t1 = MPI_Wtime();
	A = (int *)malloc(sizeof(int)*N);
	temp = (int *)malloc(sizeof(int)*N);
	temp2 = (int *)malloc(sizeof(int)*N);
	if (A == NULL) {
		printf("Fail to malloc\n");
		exit(1);
	}
//create an array of Numbers if process is 0
if (world_rank == 0) {
  for (i=N-1; i>=0; i--)
		A[N-1-i] = i;
}

	
// Create a buffer that will hold a subset of the numbers
 int *sub_bubble_nums = malloc(sizeof(int) * elements_per_proc);

// Scatter the random numbers to all processes
MPI_Scatter(A, elements_per_proc, MPI_INT, sub_bubble_nums,
            elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

// Sort the sub array with bubble sort
bubbleSort(sub_bubble_nums, elements_per_proc);
// Gather all partial averages down to the root process

MPI_Gather(sub_bubble_nums, elements_per_proc, MPI_INT, temp, elements_per_proc, MPI_INT, 0,
           MPI_COMM_WORLD);

// Compute the total average of all numbers.
if (world_rank == 0) {
mergesort(N,temp,temp2);
//bubbleSort(temp,N);
t2 = MPI_Wtime();
printf( "Elapsed time MPI_Wtime is %f\n", t2 - t1 );
}

	if (isSorted(temp, N))
	  printf("%i is sorted\n",world_rank);
	else
	  printf("Array is NOT sorted\n");
	
	 

	MPI_Finalize();
	return 0;
}
