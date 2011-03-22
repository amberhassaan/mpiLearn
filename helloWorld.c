#include <mpi.h>
#include <stdio.h>

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

  

  MPI_Finalize ();

}
