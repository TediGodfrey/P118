#include "gpluto.hpp"

#define LEFTGOING   YES

/* ********************************************************************* */
void Init (double *v, double x1, double x2, double x3, RunConfig &run_config)
/*
 * Assign initial conditions in terms of primitive variables
 *
 *********************************************************************** */
{
#ifdef GAMMA_EOS
run_config.gamma=run_config.inputParam[GAMMA_EOS];
#endif

#if LEFTGOING == YES
  if (x1 < 0.5){

    v[RHO] = run_config.inputParam[RHO_LEFT];
    v[VX1] = run_config.inputParam[VX1_LEFT];
    v[VX2] = run_config.inputParam[VX2_LEFT];
    v[VX3] = run_config.inputParam[VX3_LEFT];
    v[BX1] = run_config.inputParam[BX1_CONST];
    v[BX2] = run_config.inputParam[BX2_LEFT];
    v[BX3] = run_config.inputParam[BX3_LEFT];
    #if EOS == IDEAL
     v[PRS] = run_config.inputParam[PRS_LEFT];
    #endif

  }else{

    v[RHO] = run_config.inputParam[RHO_RIGHT];
    v[VX1] = run_config.inputParam[VX1_RIGHT];
    v[VX2] = run_config.inputParam[VX2_RIGHT];
    v[VX3] = run_config.inputParam[VX3_RIGHT];
    v[BX1] = run_config.inputParam[BX1_CONST];
    v[BX2] = run_config.inputParam[BX2_RIGHT];
    v[BX3] = run_config.inputParam[BX3_RIGHT];
    #if EOS == IDEAL
     v[PRS] = run_config.inputParam[PRS_RIGHT];
    #endif
  }

#else

  if (x1 > 0.5){

    v[RHO] =  run_config.inputParam[RHO_LEFT];
    v[VX1] = -run_config.inputParam[VX1_LEFT];
    v[VX2] =  run_config.inputParam[VX2_LEFT];
    v[VX3] =  run_config.inputParam[VX3_LEFT];
    v[BX1] = -run_config.inputParam[BX1_CONST];
    v[BX2] =  run_config.inputParam[BX2_LEFT];
    v[BX3] =  run_config.inputParam[BX3_LEFT];
    #if EOS == IDEAL
     v[PRS] = run_config.inputParam[PRS_LEFT];
    #endif

  }else{

    v[RHO] =  run_config.inputParam[RHO_RIGHT];
    v[VX1] = -run_config.inputParam[VX1_RIGHT];
    v[VX2] =  run_config.inputParam[VX2_RIGHT];
    v[VX3] =  run_config.inputParam[VX3_RIGHT];
    v[BX1] = -run_config.inputParam[BX1_CONST];
    v[BX2] =  run_config.inputParam[BX2_RIGHT];
    v[BX3] =  run_config.inputParam[BX3_RIGHT];
    #if EOS == IDEAL
    v[PRS] = run_config.inputParam[PRS_RIGHT];
    #endif
  }
#endif

  if (run_config.inputParam[DIVIDE_BY_4PI] > 0.5){
    v[BX1] /= sqrt(4.0*CONST_PI);
    v[BX2] /= sqrt(4.0*CONST_PI);
    v[BX3] /= sqrt(4.0*CONST_PI);
  }
}

/* ********************************************************************* */
void InitDomain (Data *d, Grid *grid)
/*!
 * Assign initial condition by looping over the computational domain.
 * Called after the usual Init() function to assign initial conditions
 * on primitive variables.
 * Value assigned here will overwrite those prescribed during Init().
 *
 *
 *********************************************************************** */
{
}


/* ********************************************************************* */
void UserDefBoundary (const Data *d, RBox *box, int side, Grid *grid)
/*
 *
 *
 *********************************************************************** */
{
}

/* ********************************************************************* */
void Analysis (Data *d, Grid *grid)
/* 
 *
 *
 *********************************************************************** */
{

}
