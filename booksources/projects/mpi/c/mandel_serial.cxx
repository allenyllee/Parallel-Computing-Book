#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "mandel.h"
#include "Image.h"

/**
   A class for a queue where the master sends coordinates
   to the workers with blocking sends and receives.
   Of course this is very silly, but it only serves to
   provide a code base.
 */
class serialqueue : public queue {
private :
  int free_processor;
public :
  serialqueue(MPI_Comm queue_comm,circle *workcircle)
    : queue(queue_comm,workcircle) {
    free_processor=0;
  };
  /** 
      The `addtask' routine adds a task to the queue. In this
      simple case it immediately sends the task to a worker
      and waits for the result, which is added to the image.

      This routine is only called with valid coordinates;
      the calling environment will stop the process once
      an invalid coordinate is encountered.
  */
  int addtask(struct coordinate xy) {
    MPI_Status status; int contribution, err;

    err = MPI_Send(&xy,1,coordinate_type,
	     free_processor,0,comm); CHK(err);
    err = MPI_Recv(&contribution,1,MPI_INT,
	     free_processor,0,comm, &status); CHK(err);

    coordinate_to_image(xy,contribution);
    total_tasks++;
    free_processor++;
    if (free_processor==ntids-1)
      // wrap around to the first again
      free_processor = 0;
    return 0;
  };
};

int main(int argc,char **argv) {
  MPI_Comm comm;
  int ntids,mytid, steps,iters,ierr;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);
  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  ierr = parameters_from_commandline
    (argc,argv,comm,&steps,&iters);
  if (ierr) return MPI_Abort(comm,1);

  if (ntids==1) {
    printf("Sorry, you need at least two processors\n");
    return 1;
  }

  circle *workcircle = new circle(steps,iters,ntids-1);
  serialqueue *taskqueue = new serialqueue(comm,workcircle);
  taskqueue->main_loop(comm,workcircle);

  MPI_Finalize();
  return 0;
}
