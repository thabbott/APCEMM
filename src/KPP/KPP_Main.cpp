/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                                                                  */
/* Main Program File                                                */
/*                                                                  */
/* Generated by KPP-2.2.3 symbolic chemistry Kinetics PreProcessor  */
/*       (http://www.cs.vt.edu/~asandu/Software/KPP)                */
/* KPP is distributed under GPL, the general public licence         */
/*       (http://www.gnu.org/copyleft/gpl.html)                     */
/* (C) 1995-1997, V. Damian & A. Sandu, CGRER, Univ. Iowa           */
/* (C) 1997-2005, A. Sandu, Michigan Tech, Virginia Tech            */
/*     With important contributions from:                           */
/*        M. Damian, Villanova University, USA                      */
/*        R. Sander, Max-Planck Institute for Chemistry, Mainz, Germany */
/*                                                                  */
/* File                 : KPP_Main.c                                */
/* Time                 : Tue Sep  4 17:12:00 2018                  */
/* Equation file        : KPP.kpp                                   */
/* Output root filename : KPP                                       */
/*                                                                  */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "KPP/KPP.hpp"
#include "KPP/KPP_Parameters.h"
#include "KPP/KPP_Global.h"
#include "KPP/KPP_Sparse.h"

double C[NSPEC];                         /* Concentration of all species */
double * VAR = & C[0];
double * FIX = & C[127];
double RCONST[NREACT];                   /* Rate constants (global) */
double PHOTOL[NPHOTOL];                  /* Photolysis rates (global) */
double HET[NSPEC][3];                    /* Heterogeneous chemistry rates (global) */
double TIME;                             /* Current integration time */
double RTOLS;                            /* (scalar) Relative tolerance */
double TSTART;                           /* Integration start time */
double TEND;                             /* Integration end time */
double ATOL[NVAR];                       /* Absolute tolerance */
double RTOL[NVAR];                       /* Relative tolerance */
double STEPMIN;                          /* Lower bound for integration step */
double STEPMAX;                          /* Upper bound for integration step */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                                                                  */
/* MAIN - Main program - driver routine                             */
/*   Arguments :                                                    */
/*                                                                  */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void Read_JRates( double JRates[] );
void GetMass( double CL[], double Mass[] );
void INTEGRATE( double TIN, double TOUT );

int KPP_Main( double varArray[], double fixArray[], double currentT, double dt, \
              double RTOLS, double ATOLS )
{
    
    int i;
 
    /* ---- TIME VARIABLES ------------------ */

    TSTART = currentT; // Solar time
    TEND = TSTART + dt;

    for ( i = 0; i < NVAR; i++ ) {
        VAR[i] = varArray[i];
    }

    for ( i = 0; i < NFIX; i++ ) {
        FIX[i] = fixArray[i];
    }

    for( i = 0; i < NVAR; i++ ) {
        RTOL[i] = RTOLS;
        ATOL[i] = ATOLS;
    }
    STEPMIN = 0.01;
    STEPMAX = 900;
      
    /* ********** TIME LOOP **************************** */

    TIME = TSTART;
    INTEGRATE( TIME, TIME+dt );

    /* *********** END TIME LOOP *********************** */

    for ( i = 0; i < NVAR; i++ ) {
        varArray[i] = VAR[i];
    }

    return 0; /*didnt return anything initially */

}
/* End of MAIN function                                             */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


