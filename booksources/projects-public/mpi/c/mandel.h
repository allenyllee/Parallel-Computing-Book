#ifndef MANDEL_H
#define MANDEL_H 1

#include <mpi.h>
#include "Image.h"
#include "tools.h"

#define CHK(x) if (x) {							\
    char errtxt[200]; int len=200, mytid;				\
    MPI_Error_string(x,errtxt,&len);					\
    MPI_Comm_rank(MPI_COMM_WORLD,&mytid);				\
    printf("p=%d, line=%d, err=%d, %s\n",mytid,__LINE__,x,errtxt);	\
  return x;}

int parameters_from_commandline(int argc,char** argv,MPI_Comm comm,
				int *rsteps,int *riter);
void make_coordinate_type(MPI_Datatype *ctype);

struct coordinate { double x,y; } ;
int belongs(struct coordinate xy,int itbound);

class circle {
private:
  int readpointer,writepointer;
  struct coordinate *block;
  void next_coordinate_internal(struct coordinate& xy);
  MPI_Datatype coordinate_type;
public:
  double step; int infty,blocksize,pixels;
private:
  double xmin,xmax,ymin,ymax, x,y;
 public :
  circle(int pxls,int bound,int bs);
  void next_coordinate(struct coordinate& xy);
  int is_valid_coordinate(struct coordinate xy);
  void invalid_coordinate(struct coordinate& xy);
};

class queue {
protected:
  int ntids,total_tasks,t_start,t_stop;
  MPI_Comm comm;
  circle *workcircle;
  Image *image;
  MPI_Datatype coordinate_type;
  double area;
public:
  queue(MPI_Comm queue_comm,circle *disccircle) {
    comm = queue_comm; workcircle = disccircle;
    total_tasks = 0;
    MPI_Comm_size(comm,&ntids);
    make_coordinate_type(&coordinate_type);
    area = 0.;
    t_start = MPI_Wtime();
  };
  virtual void addtask(struct coordinate) = 0;
  void set_image(Image*);
  void complete();
  void coordinate_to_image(struct coordinate,int);
  void wait_for_work(MPI_Comm,circle*);
};

void scan_circle(queue *taskqueue);

void main_loop(MPI_Comm,circle*,queue*);

#endif
