#if defined(TOOLS_H)
#else
#define TOOLS_H 1

int error(MPI_Comm comm,int id,const char *fmt,...);
int commandline_argument(MPI_Comm,int,char**,const char*,int);

#endif
