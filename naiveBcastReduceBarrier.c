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

  printf("processor %d sending its time to proc 0\n", myid);
  MPI_Send (&time, 1, MPI_DOUBLE, 0, myid, MPI_COMM_WORLD);

  double totalTime = 0;
  double recvTime;
  if (myid == 0) { 
    int i;
    MPI_Status status;
    for (i = 0; i < numprocs; ++i) {
      MPI_Recv (&recvTime, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
      totalTime += recvTime;
      printf ("received time = %g from proc = %d\n", recvTime, i);
    }

    printf ("time taken, total = %g, avg=%g\n", totalTime, (totalTime/numprocs));

    // proc 0 sends the average time to everyone
    double avgTime = totalTime / numprocs;
    for (i = 0; i < numprocs; ++i) {
      printf ("Proc 0 sending avgTime to proc = %d\n", i);
      MPI_Send (&avgTime, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
    }
  } // end if proc 0

  MPI_Barrier (MPI_COMM_WORLD);

  double avgTime;
  MPI_Status status;
  MPI_Recv (&avgTime, 1, MPI_DOUBLE, 0, myid, MPI_COMM_WORLD, &status);

  printf ("Proc = %d received average time = %g, from proc 0\n", myid, avgTime);

  MPI_Finalize ();
  
}
