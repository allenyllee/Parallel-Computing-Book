#include "petsc.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  MPI_Comm       comm;
  PetscInt       n = 20;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscInitialize(&argc,&argv,0,"\nUsage: init [-n number]\n\n"); CHKERRQ(ierr);
  {
    PetscBool has;
    ierr = PetscOptionsHasName(PETSC_NULL,"-help",&has); CHKERRQ(ierr);
    if (has) goto skip;
  }
  comm = PETSC_COMM_WORLD;
  ierr = PetscOptionsGetInt(PETSC_NULL,"-n",&n,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscPrintf(comm,"Input parameter: %d\n",n); CHKERRQ(ierr);
 skip:
  ierr = PetscFinalize();CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

