#include <mpi.h>
#include <stdio.h>

double f (double x) {
  return 4.0 / ( 1 + x * x);
}

int main (int argc, char** argv) {

  double PI25DT = 3.141592653589793238462643;

  MPI_Init (&argc, &argv);

  int myid;

  int numprocs;

  int namelen;
  char myname[MPI_MAX_PROCESSOR_NAME];


  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);

  MPI_Comm_rank (MPI_COMM_WORLD, &myid);

  MPI_Get_processor_name (myname, &namelen);



  printf ("Hello World from process %d (%s) out of %d\n", myid, myname, numprocs);

  // compute PI

  int N;
  if (myid == 0) {
    printf ("enter num intervals on proc 0\n");
    scanf ("%d", &N);
  }

  MPI_Barrier (MPI_COMM_WORLD);

  double time = MPI_Wtime ();

  MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

  double mypi = 0.0;
  
  int i;

  for ( i = myid; i < N; i += numprocs) {

    // width of each rectangular section
    double w = 1.0 / (double)N;

    // mid point of the rectangle
    double x_i = ((double)i + 0.5) / (double)N;

    // height of the curve at the mid point
    double h = f(x_i);

    // area of the rectangle
    mypi  += w * h;
  }
  
  printf ("proc %d contributed %10.25f to pi\n", myid, mypi);

  double pi = 0.0;

  MPI_Reduce (&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Barrier (MPI_COMM_WORLD);

  if (myid == 0) {
    printf ("Computed value of pi = %10.25f\n", pi);

    printf ("Actual   value of pi = %10.25f\n", PI25DT);

    double err = (PI25DT - pi) / PI25DT;

    printf ("Relative error = %g\n", err);


  }

  time = MPI_Wtime () - time;

  printf ("Time taken by proc %d is %g\n", myid, time);

  MPI_Finalize();
}
