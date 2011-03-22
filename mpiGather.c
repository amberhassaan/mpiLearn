#include <mpi.h>
#include <stdio.h>
#include <math.h>

void printArgs (int argc, char** argv) {
  int i;
  for (i = 0; i < argc; ++i) {
    printf ("argv[%d] = %s\n", i, argv[i]);
  }
}

double doWork (int niter) {
  int i;
  double result;
  for (i = 0; i < niter; ++i) {
    result += sqrt ((double) i);
  }
  return result;
}

int main (int argc, char** argv) {

  MPI_Init (&argc, &argv);

  int numprocs;
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);

  int myid;
  MPI_Comm_rank (MPI_COMM_WORLD, &myid);
  
  printf ("Hello World from processor: %d out of %d\n", myid, numprocs);

  int namelen;
  char myname[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name (myname, &namelen);
  printf ("processor id=%d, name=%s\n", myid, myname);

  double time = MPI_Wtime ();

  double d = doWork (1e7);

  time = MPI_Wtime() - time;

  printf ("Time taken by proc = %d is = %g\n", myid, time);

  double totalTime = 0;
  double* recvTime;
  
  if (myid == 0) {
    recvTime = (double*) malloc (numprocs * sizeof(double));
  }

  MPI_Gather (
      &time, 1, MPI_DOUBLE, 
      recvTime, 1, MPI_DOUBLE, 
      0, MPI_COMM_WORLD);


  if (myid == 0) {
    int i;
    for (i = 0; i < numprocs; ++i) {
      totalTime += recvTime[i];
    }
    printf ("Total time = %g, avg = %g\n", totalTime, (totalTime/numprocs));

    free (recvTime);
  }

  // proc 0 send the average time to everyone
  double avgTime = 0;

  avgTime = totalTime / numprocs;
  
  MPI_Bcast (&avgTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  printf ("proc %d received avg time = %g from proc 0\n", myid, avgTime);
  

  MPI_Finalize ();
  
}
