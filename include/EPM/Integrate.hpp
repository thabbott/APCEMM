/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                                                                  */
/*                      Early Plume Microphysics                    */
/*                              (EPM)                               */
/*                                                                  */
/* Integrate Header File                                            */
/*                                                                  */
/* Author               : Thibaud M. Fritz                          */
/* Time                 : 9/27/2018                                 */
/* File                 : Integrate.hpp                             */
/*                                                                  */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef INTEGRATE_H_INCLUDED
#define INTEGRATE_H_INCLUDED

#include <iostream>
#include <cmath>
#include <vector>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

#include "Util/ForwardsDecl.hpp"
#include "Util/PhysConstant.hpp"
#include "Util/PhysFunction.hpp"
#include "Core/Interface.hpp"
#include "Core/Parameters.hpp"
#include "Core/Monitor.hpp"
#include "Core/Aircraft.hpp"
#include "Core/Emission.hpp"
#include "AIM/Coagulation.hpp"
#include "AIM/Aerosol.hpp"
#include "odeSolver.hpp"

namespace EPM
{

    static const int EPM_SUCCESS = 1;
    static const int EPM_FAILURE = 0;

    /* Vortex sinking timescales, taken from Unterstrasser et al., 2008 */
    const RealDouble t_Vortex_0 = 8.0;
    const RealDouble t_Vortex_1 = 110.0;

    /* Dilution timescales for a B747, taken from:
     * B. Kärcher, "A trajectory box model for aircraft exhaust plumes", Journal of Geophysical Research, 1995 */
    const RealDouble t_0 = 1.00E-04; /* [s] */
    const RealDouble t_j = 1.00E-02; /* [s] */
    const RealDouble t_1 = 8.00E+00; /* [s] */
    const RealDouble t_2 = 6.60E+02; /* [s] */
   
    const RealDouble m = 2.0;
    const RealDouble n = 50.0;
    const RealDouble Cv = 3.0;
    
    /* Engine exit plane characteristics for a B747, taken from:
     * B. Kärcher, "A trajectory box model for aircraft exhaust plumes", Journal of Geophysical Research, 1995 */
    /* Engine exit core area im m^2 */
    const RealDouble Ac0 = 0.604;
    /* Engine exit core velocity in m/s */
    const RealDouble uc0 = 475.7;
    /* Engine exit core temperature in K */
    const RealDouble Tc0 = 547.3;
    /* Engine exit bypass area in m^2 */
    const RealDouble Ab0 = 1.804;


    int Integrate( RealDouble temperature_K, RealDouble pressure_Pa, RealDouble relHumidity_w, RealDouble varArray[], RealDouble fixArray[], const Aircraft &ac, const Emission &EI );
    int RunMicrophysics( RealDouble temperature_K, RealDouble pressure_Pa, RealDouble relHumidity_w, RealDouble varArray[], RealDouble fixArray[], const Aircraft &ac, const Emission &EI, RealDouble delta_T_ad, RealDouble delta_T );
    RealDouble temp_Vortex( const RealDouble time, const RealDouble delta_T );
    RealDouble dilutionRatio( const RealDouble time );
    RealDouble depositionRate( const RealDouble r, const RealDouble T, const RealDouble P, const RealDouble H2O );
    void odeRHS( const Vector_1D &x, Vector_1D &dxdt, const RealDouble t = 0.0 );


}


#endif /* INTEGRATE_H_INCLUDED */
