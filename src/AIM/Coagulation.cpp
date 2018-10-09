/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                                                                  */
/*                        AIrcraft Microphysics                     */
/*                              (AIM)                               */
/*                                                                  */
/* Coagulation Program File                                         */
/*                                                                  */
/* Author               : Thibaud M. Fritz                          */
/* Time                 : 9/27/2018                                 */
/* File                 : Coagulation.cpp                           */
/*                                                                  */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "AIM/Coagulation.hpp"

AIM::Coagulation::Coagulation( )
{

    /* Default Constructor */

} /* End of Coagulation::Coagulation */

AIM::Coagulation::Coagulation( const char* phase, Vector_1D const &bin_Centers_1, Vector_1D const &bin_VCenters_1, RealDouble rho_1, Vector_1D const &bin_Centers_2, RealDouble rho_2, RealDouble temperature_K_, RealDouble pressure_Pa_ ): Kernel( bin_Centers_1.size() )
{

    /* Constructor */

    RealDouble temperature_K = temperature_K_;
    RealDouble pressure_Pa = pressure_Pa_;

    /* Set shape */
    for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
        Kernel[iBin_1].resize( bin_Centers_2.size() );
    }

    if ( ( strcmp ( phase, "liq" ) == 0) || ( strcmp( phase, "liquid" ) == 0) || ( strcmp( phase, "sulfate" ) == 0) ) {
        /* Liquid-liquid coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_2D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_2D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2, K_Brow );

        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            for ( unsigned int iBin_2 = 0; iBin_2 < bin_Centers_2.size(); iBin_2++ ) {
                Kernel[iBin_1][iBin_2] = ( K_Brow[iBin_1][iBin_2] + K_DE[iBin_1][iBin_2] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
            }
        }

    } 
    else if ( ( strcmp ( phase, "ice" ) == 0) ) { 
        /* Ice-Ice coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_2D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_2D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2, K_Brow );
        Vector_2D K_GC   = buildGCKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_2D K_TI   = buildTIKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_2D K_TS   = buildTSKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        
        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            for ( unsigned int iBin_2 = 0; iBin_2 < bin_Centers_2.size(); iBin_2++ ) {
                Kernel[iBin_1][iBin_2] = ( K_Brow[iBin_1][iBin_2] + K_DE[iBin_1][iBin_2] + K_GC[iBin_1][iBin_2] + K_TI[iBin_1][iBin_2] + K_TS[iBin_1][iBin_2] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
            }
        }

    }
    else if ( ( strcmp ( phase, "soot" ) == 0) || ( strcmp( phase, "bc" ) == 0) ) {
        /* Soot-soot coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_2D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_2D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2, K_Brow );

        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            for ( unsigned int iBin_2 = 0; iBin_2 < bin_Centers_2.size(); iBin_2++ ) {
                Kernel[iBin_1][iBin_2] = ( K_Brow[iBin_1][iBin_2] + K_DE[iBin_1][iBin_2] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
            }
        }

    }
    else {
        std::cout << "\nIn AIM::Coagulation::Coagulation: phase " << phase << " is not defined.";
        std::cout << "\nOptions are: liquid, ice or soot.";
    }

    buildBeta( bin_Centers_1 );
    buildF   ( bin_VCenters_1 );


} /* End of Coagulation::Coagulation */

AIM::Coagulation::Coagulation( const char* phase, Vector_1D const &bin_Centers_1, Vector_1D const &bin_VCenters_1, RealDouble rho_1, RealDouble temperature_K_, RealDouble pressure_Pa_ ): Kernel( bin_Centers_1.size() )
{

    /* Constructor */

    RealDouble temperature_K = temperature_K_;
    RealDouble pressure_Pa = pressure_Pa_;

    /* Set shape */
    for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
        Kernel[iBin_1].resize( bin_Centers_1.size() );
    }

    if ( ( strcmp ( phase, "liq" ) == 0) || ( strcmp( phase, "liquid" ) == 0) || ( strcmp( phase, "sulfate" ) == 0) ) {
        /* Liquid-liquid coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_2D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1 );
        Vector_2D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1, K_Brow );

        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            for ( unsigned int iBin_2 = 0; iBin_2 < bin_Centers_1.size(); iBin_2++ ) {
                Kernel[iBin_1][iBin_2] = ( K_Brow[iBin_1][iBin_2] + K_DE[iBin_1][iBin_2] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
            }
        }

    } 
    else if ( ( strcmp ( phase, "ice" ) == 0) ) { 
        /* Ice-Ice coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_2D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1 );
        Vector_2D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1, K_Brow );
        Vector_2D K_GC   = buildGCKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1 );
        Vector_2D K_TI   = buildTIKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1 );
        Vector_2D K_TS   = buildTSKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1 );
        
        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            for ( unsigned int iBin_2 = 0; iBin_2 < bin_Centers_1.size(); iBin_2++ ) {
                Kernel[iBin_1][iBin_2] = ( K_Brow[iBin_1][iBin_2] + K_DE[iBin_1][iBin_2] + K_GC[iBin_1][iBin_2] + K_TI[iBin_1][iBin_2] + K_TS[iBin_1][iBin_2] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
            }
        }

    }
    else if ( ( strcmp ( phase, "soot" ) == 0) || ( strcmp( phase, "bc" ) == 0) ) {
        /* Soot-soot coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_2D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1 );
        Vector_2D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_1, rho_1, K_Brow );

        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            for ( unsigned int iBin_2 = 0; iBin_2 < bin_Centers_1.size(); iBin_2++ ) {
                Kernel[iBin_1][iBin_2] = ( K_Brow[iBin_1][iBin_2] + K_DE[iBin_1][iBin_2] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
            }
        }

    }
    else {
        std::cout << "\nIn AIM::Coagulation::Coagulation: phase " << phase << " is not defined.";
        std::cout << "\nOptions are: liquid, ice or soot.";
    }

    buildBeta( bin_Centers_1 );
    buildF   ( bin_VCenters_1 );

} /* End of Coagulation::Coagulation */
        
AIM::Coagulation::Coagulation( const char* phase, Vector_1D const &bin_Centers_1, RealDouble rho_1, RealDouble bin_Centers_2, RealDouble rho_2, RealDouble temperature_K_, RealDouble pressure_Pa_ ):
    Kernel_1D( bin_Centers_1.size() )
{

    /* Constructor */

    RealDouble temperature_K = temperature_K_;
    RealDouble pressure_Pa = pressure_Pa_;

    /* Declare coagulation kernel and set shape */

    if ( ( strcmp ( phase, "liq" ) == 0) || ( strcmp( phase, "liquid" ) == 0) ) {
        /* Liquid-liquid coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_1D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_1D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2, K_Brow );

        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            Kernel_1D[iBin_1] = ( K_Brow[iBin_1] + K_DE[iBin_1] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
        }

    } 
    else if ( ( strcmp ( phase, "ice" ) == 0) ) { 
        /* Ice-Ice coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_1D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_1D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2, K_Brow );
        Vector_1D K_GC   = buildGCKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_1D K_TI   = buildTIKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_1D K_TS   = buildTSKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        
        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            Kernel_1D[iBin_1] = ( K_Brow[iBin_1] + K_DE[iBin_1] + K_GC[iBin_1] + K_TI[iBin_1] + K_TS[iBin_1] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
        }

    }
    else if ( ( strcmp ( phase, "soot" ) == 0) || ( strcmp( phase, "bc" ) == 0) ) {
        /* Soot-soot coagulation */

        /* Declare and assign coagulation kernels for different physical processes */
        Vector_1D K_Brow = buildBrownianKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2 );
        Vector_1D K_DE   = buildDEKernel( temperature_K, pressure_Pa, bin_Centers_1, rho_1, bin_Centers_2, rho_2, K_Brow );

        /* Fill in total coagulation kernel */
        for ( unsigned int iBin_1 = 0; iBin_1 < bin_Centers_1.size(); iBin_1++ ) {
            Kernel_1D[iBin_1] = ( K_Brow[iBin_1] + K_DE[iBin_1] ) * 1.00E+06; /* Conversion from m^3/s to cm^3/s */
        }

    }
    else {
        std::cout << "\nIn AIM::Coagulation::Coagulation: phase " << phase << " is not defined.";
        std::cout << "\nOptions are: liquid, ice or soot.";
    }

} /* End of Coagulation::Coagulation */

AIM::Coagulation::~Coagulation( )
{

    /* Destructor */

} /* End of Coagulation::~Coagulation */

AIM::Coagulation::Coagulation( const Coagulation &k )
{

    Kernel = k.Kernel;
    Kernel_1D = k.Kernel_1D;

} /* End of Coagulation::Coagulation */

AIM::Coagulation& AIM::Coagulation::operator=( const Coagulation &k )
{

    if ( &k == this )
        return *this;

    Kernel = k.Kernel;
    Kernel_1D = k.Kernel_1D;
    return *this;

} /* End of Coagulation::operator= */

void AIM::Coagulation::buildBeta( const Vector_1D &bin_Centers )
{

    Vector_1D E_coal( bin_Centers.size() );
    Vector_1D E_prev( bin_Centers.size() );
    RealDouble r1, r2;

    /* For Newton-Raphson iteration process */
    bool notConverged = 1;
    RealDouble metric = 0;
    
    for ( UInt iBin = 0; iBin < bin_Centers.size(); iBin++ ) {
        beta.push_back( Vector_1D( bin_Centers.size() ) );

        for ( UInt jBin = 0; jBin < bin_Centers.size(); jBin++ ) {
            E_coal[jBin] = 0.5; 
            E_prev[jBin] = 0.0;

            /* Get smallest radius in micrometers */
            r1 = std::min(bin_Centers[iBin], bin_Centers[jBin]) * 1.0E+06; /* [mum] */
            
            /* Get largest radius in micrometers */
            r2 = std::max(bin_Centers[iBin], bin_Centers[jBin]) * 1.0E+06; /* [mum] */

            /* Perform Newton-Raphson iteration */
            notConverged = 1;
            metric = 0.0;
            while ( notConverged ) {

                E_prev[jBin] = E_coal[jBin];
                E_coal[jBin] = std::max( std::min( E_coal[jBin] - \
                               ( A0 + E_coal[jBin] * ( A1 + E_coal[jBin] * ( A2 + E_coal[jBin] * A3 )) - log( r1 ) - log( r2 / RealDouble(200.0) ) ) / ( A1 + E_coal[jBin] * ( 2.0 * A2 + E_coal[jBin] * 3.0 * A3 ) ), 1.0 ), 0.0 );

                metric = std::abs( E_coal[jBin] - E_prev[jBin] );
                notConverged = ( metric > 1.0E-03 );
            }

            beta[iBin][jBin] = E_coal[jBin] * Kernel[iBin][jBin];
        }
    }

} /* End of Coagulation::buildBeta */

void AIM::Coagulation::buildF( const Vector_1D &bin_VCenters )
{

    RealDouble vij;
    UInt iBin, jBin, kBin, index;
    UInt size = bin_VCenters.size();
    bool notFound;

    Vector_2D v2d;

    for ( iBin = 0; iBin < size; iBin++ ) {
        f.push_back( v2d );
        for ( jBin = 0; jBin < size; jBin++ ) {
            f[iBin].push_back( Vector_1D( size, 0.0 ) );
            vij = bin_VCenters[iBin] + bin_VCenters[jBin];
            notFound = 1;
            kBin = 0;
            while ( notFound ) {
                if ( bin_VCenters[kBin] >= vij ) {
                    index = kBin - 1;
                    notFound = 0;
                } else {
                    kBin++;
                }
                if ( kBin >= size - 1 ) {
                    index = size - 1;
                    notFound = 0;
                }
            }
            if ( index < size - 1 ) {
                f[iBin][jBin][index+1] = ( bin_VCenters[index+1] - vij ) / ( bin_VCenters[index+1] - bin_VCenters[index] ) * bin_VCenters[index] / vij;
            } else if ( index == size - 1 ) {
                f[iBin][jBin][index] = 1.0;
            }
        }
    }

} /* End of Coagulation::buildF */

Vector_2D AIM::Coagulation::getKernel() const
{

    return Kernel;

} /* End of Coagulation::getKernel */
        
Vector_1D AIM::Coagulation::getKernel_1D() const
{

    return Kernel_1D;

} /* End of Coagulation::getKernel_1D */

Vector_2D AIM::Coagulation::getBeta() const
{

    return beta;

} /* End of Coagulation::getBeta */

Vector_3D AIM::Coagulation::getF() const
{

    return f;

} /* End of Coagulation::getF */

void AIM::Coagulation::printKernel_1D( const char* fileName ) const
{

    printKernel2File( Kernel_1D, fileName ); 

    return;

} /* End of Coagulation::printKernel_1D */

void AIM::Coagulation::printKernel_2D( const char* fileName) const
{

    printKernel2File( Kernel, fileName ); 

    return;

} /* End of Coagulation::printKernel_2D */

/* End of Coagulation.cpp */
