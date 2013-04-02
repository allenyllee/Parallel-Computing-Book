#include "petsc.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  MPI_Comm       comm;
  int ntids,mytid,myfirst,mylast,localsize,i;
  Vec            x;
  Mat            A;
  PetscInt       n = 20;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscInitialize(&argc,&argv,0,"\nUsage: mul [-n number]\n\n"); CHKERRQ(ierr); 
  {
    PetscBool has;
    ierr = PetscOptionsHasName(PETSC_NULL,"-help",&has); CHKERRQ(ierr);
    if (has) goto skip;
  }
  comm = PETSC_COMM_WORLD;
  ierr = PetscOptionsGetInt(PETSC_NULL,"-n",&n,PETSC_NULL); CHKERRQ(ierr);

  MPI_Comm_size(comm,&ntids); MPI_Comm_rank(comm,&mytid);

  ierr = VecCreate(comm,&x);CHKERRQ(ierr);
  ierr = VecSetSizes(x,PETSC_DECIDE,n); CHKERRQ(ierr);
  ierr = VecSetType(x,VECMPI); CHKERRQ(ierr);
  ierr = VecSetUp(x); CHKERRQ(ierr);
  ierr = VecGetOwnershipRange(x,&myfirst,&mylast); CHKERRQ(ierr);
  localsize = mylast-myfirst;

  ierr = MatCreate(comm,&A); CHKERRQ(ierr);
  ierr = MatSetType(A,MATMPIAIJ); CHKERRQ(ierr);
  ierr = MatSetSizes(A,localsize,localsize,
		     PETSC_DECIDE,PETSC_DECIDE); CHKERRQ(ierr);
  ierr = MatMPIAIJSetPreallocation(A,3,PETSC_NULL,1,PETSC_NULL); CHKERRQ(ierr);

  for (i=myfirst; i<mylast; i++) {
    PetscReal v=1.0*mytid;
    ierr = MatSetValues(A,1,&i,1,&i,&v,INSERT_VALUES); CHKERRQ(ierr);
  }
  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
  ierr = MatView(A,0); CHKERRQ(ierr);

 skip:
  ierr = PetscFinalize();CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

