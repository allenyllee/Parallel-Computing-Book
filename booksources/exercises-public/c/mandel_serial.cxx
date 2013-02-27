#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "mandel.h"
#include "Image.h"

class serialqueue : public queue {
private :
  int free_processor;
public :
  serialqueue(MPI_Comm queue_comm,circle *workcircle)
    : queue(queue_comm,workcircle) {
    free_processor=0;
  };
  /* Send a coordinate to a free processor;
     if the coordinate is invalid, this should stop the process;
     otherwise add the result to the image.
  */
  void addtask(struct coordinate xy) {
    MPI_Status status; int contribution;

    MPI_Send(&xy,2,MPI_DOUBLE, 
	     free_processor,0,comm);
    MPI_Recv(&contribution,1,MPI_INT,
	     free_processor,0,comm, &status);
    if (workcircle->is_valid_coordinate(xy)) {
      coordinate_to_image(xy,contribution);
      total_tasks++;
    }
    free_processor++;
    if (free_processor==ntids-1)
      // wrap around to the first again
      free_processor = 0;
  };
  void complete(circle *workcircle) { 
    struct coordinate xy;
    workcircle->invalid_coordinate(xy); free_processor=0;
    for (int p=0; p<ntids-1; p++)
      addtask(xy);
    t_stop = MPI_Wtime();
    // printf("Area computed: %e\n by %d tasks in time %d\n",
    // 	   area,total_tasks,t_stop-t_start);
    image->Write();
    return; 
  };
};

int main(int argc,char **argv) {
  MPI_Comm comm;
  int ntids,mytid, steps,iters,ierr;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  ierr = parameters_from_commandline
    (argc,argv,comm,&steps,&iters);
  if (ierr) return MPI_Abort(comm,1);

  if (ntids==1) {
    printf("Sorry, you need at least two processors\n");
    return 1;
  }

  circle *workcircle = new circle(2./steps,iters);
  serialqueue *taskqueue = new serialqueue(comm,workcircle);
  if (mytid==ntids-1)  {
    taskqueue->set_image( new Image(2*steps,2*steps,"mandelpicture") );
    for (;;) {
      struct coordinate xy;
      workcircle->next_coordinate(xy);
      if (workcircle->is_valid_coordinate(xy))
	taskqueue->addtask(xy);
      else break;
    }
    taskqueue->complete(workcircle);
  } else
    taskqueue->wait_for_work(comm,workcircle);

  MPI_Finalize();
  return 0;
}
