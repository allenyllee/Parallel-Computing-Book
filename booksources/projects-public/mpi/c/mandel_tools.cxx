#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tools.h"
#include "mandel.h"
#include "unistd.h"

int parameters_from_commandline(int argc,char** argv,MPI_Comm comm,
				int *rsteps,int *riters)
{
  int mytid,ntids, steps,iters;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  if (mytid==0) {
    if ( argc>1 && ( !strcmp(argv[1],"-h") || !strcmp(argv[1],"h") ) )
      return error(comm,0,"\nUsage: mandel steps 123 iters 456\n");
    steps = commandline_argument(argc,argv,"steps",10);
    iters = commandline_argument(argc,argv,"iters",100000);
  }
  MPI_Bcast(&steps,1,MPI_INT,0,comm);
  MPI_Bcast(&iters,1,MPI_INT,0,comm);
  *rsteps = steps; *riters = iters;
  return 0;
}

circle::circle(double stp,int bound) {
  infty = bound;
  xmin=-2.; xmax=+2.; step=stp; x = xmin;
  ymax = sqrt(4-x*x); ymin = -ymax; y = ymin;
}
void circle::next_coordinate(struct coordinate& xy) {
  if (y<ymax-step) {
    xy.x = x; xy.y = y;
    y += step;
  } else if (x<xmax-step) {
    x += step; 
    ymax = sqrt(4-x*x); ymin = -ymax; y = ymin;
    xy.x = x; xy.y = y; y += step;
  } else {
    invalid_coordinate(xy); //.x = -5.; xy.y = -5;
  }
  return;
}
int circle::is_valid_coordinate(struct coordinate xy) {
  return xy.x>-3. && xy.y > -3.;
}
void circle::invalid_coordinate(struct coordinate& xy) {
  xy.x = -5.; xy.y = -5.;
}

int belongs(struct coordinate xy,int itbound) {
  double x=xy.x, y=xy.y; int it;
  for (it=0; it<itbound; it++) {
    double xx,yy;
    xx = x*x - y*y + xy.x;
    yy = 2*x*y + xy.y;
    x = xx; y = yy;
    if (x*x+y*y>4.) {
      //sleep(1);
      return 0;
    }
  }
  return 1;
}

void queue::wait_for_work(MPI_Comm comm,circle *workcircle) {
  MPI_Status status; int ntids;
  MPI_Comm_size(comm,&ntids);
  int stop = 0;

  while (!stop) {
    struct coordinate xy;
    double step = workcircle->step,res;

    MPI_Recv(&xy,2,MPI_DOUBLE,ntids-1,0, comm,&status);
    stop = !workcircle->is_valid_coordinate(xy);
    if (stop) res = 0.;
    else
      res = belongs(xy,workcircle->infty)*step*step;
    MPI_Send(&res,1,MPI_DOUBLE,ntids-1,0, comm);
  }
  return;
}
