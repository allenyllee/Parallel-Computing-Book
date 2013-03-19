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
  ymin=-2.; ymax=+2.; step=stp; y = ymin;
  xmax = sqrt(4-y*y); xmin = -xmax; x = xmin;
}

/* Generate successive coordinates in the circle */
void circle::next_coordinate(struct coordinate& xy) {
  if (x<xmax-step) {
    xy.x = x; xy.y = y; x += step;
  } else if (y<ymax-step) {
    y += step; 
    xmax = sqrt(4-y*y); xmin = -xmax; x = xmin;
    xy.x = x; xy.y = y; x += step;
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

/* Compute how many iterations it takes for the Mandelbrot
   iteration to escape the circle, return 0 otherwise
*/
int belongs(struct coordinate xy,int itbound) {
  double x=xy.x, y=xy.y; int it;
  for (it=0; it<itbound; it++) {
    double xx,yy;
    xx = x*x - y*y + xy.x;
    yy = 2*x*y + xy.y;
    x = xx; y = yy;
    if (x*x+y*y>4.) {
      return it;
    }
  }
  return 0;
}

void queue::set_image(Image *theimage) {
  image = theimage;
}

void queue::wait_for_work(MPI_Comm comm,circle *workcircle) {
  MPI_Status status; int ntids;
  MPI_Comm_size(comm,&ntids);
  int stop = 0;

  while (!stop) {
    struct coordinate xy;
    int res;

    MPI_Recv(&xy,2,MPI_DOUBLE,ntids-1,0, comm,&status);
    stop = !workcircle->is_valid_coordinate(xy);
    if (stop) res = 0;
    else {
      res = belongs(xy,workcircle->infty);
    }
    MPI_Send(&res,1,MPI_INT,ntids-1,0, comm);
  }
  return;
}

void queue::coordinate_to_image(struct coordinate xy,int iteration) {
  int nx = (int) ( (xy.x+2.f)*(float)image->width*.25 ), 
    ny = (int) ( (xy.y+2.f)*(float)image->height*.25 );
  int pixel = nx+ny*image->width;
  float colour[3];
  if (iteration==0)
    memset(colour,0,3*sizeof(float));
  else {
    float rfloat = ((float) iteration) / workcircle->infty;
    colour[0] = rfloat;
    colour[1] = max((float)0,(float)(1-2*rfloat));
    colour[2] = max((float)0,(float)(2*(rfloat-.5)));
  }
  image->Add( pixel, colour );
}

