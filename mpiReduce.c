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
  double recvTime;

  MPI_Reduce (&time, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (myid == 0) {
    printf ("Total time = %g, avg = %g\n", totalTime, (totalTime/numprocs));
  }


  MPI_Finalize ();
  
}
