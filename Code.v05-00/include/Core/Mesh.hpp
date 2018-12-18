/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                                                                  */
/*     Aircraft Plume Chemistry, Emission and Microphysics Model    */
/*                             (APCEMM)                             */
/*                                                                  */
/* Mesh Header File                                                 */
/*                                                                  */
/* Author               : Thibaud M. Fritz                          */
/* Time                 : 7/26/2018                                 */
/* File                 : Mesh.hpp                                  */
/*                                                                  */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <vector>

#include "Core/Parameters.hpp"
#include "Core/Interface.hpp"
#include "Core/Cluster.hpp"
#include "Core/Ring.hpp"

typedef double RealDouble;
typedef std::vector<RealDouble> Real_1DVector;
typedef std::vector<Real_1DVector> Real_2DVector;


class Mesh
{
    public:

        Mesh( );
        ~Mesh( );
        Mesh( const Mesh &m );
        Mesh& operator=( const Mesh &m );
        void Ring2Mesh( Cluster &c );
        Real_1DVector x( ) const { return x_; }
        Real_1DVector y( ) const { return y_; }
        Real_1DVector x_edge( ) const { return x_e_; }
        Real_1DVector y_edge( ) const { return y_e_; }
        Real_2DVector areas( ) const { return areas_; }
        RealDouble totArea( ) const { return totArea_; }
        RealDouble hx( ) const { return hx_; }
        RealDouble hy( ) const { return hy_; }
        unsigned int Nx() const { return nx; }
        unsigned int Ny() const { return ny; }
        std::vector<std::vector<std::vector<bool> > > map( ) const { return RingMeshMap; }
        std::vector<std::vector<std::pair<unsigned int, unsigned int> > > list() const { return indList; }
        std::vector<unsigned int> nMap( ) const { return nCellMap; }
        void Debug() const;

    private:

        /* Cell center coordinates */
        Real_1DVector x_, y_;

        /* Cell edges */
        Real_1DVector x_e_, y_e_;

        /* Cell areas */
        Real_2DVector areas_;

        /* Total area */
        RealDouble totArea_;

        RealDouble xlim, ylim;
        RealDouble hx_, hy_;
        unsigned int nx, ny;
        std::vector<unsigned int> nCellMap;
        std::vector<std::vector<std::pair<unsigned int, unsigned int> > > indList;
        std::vector<std::vector<std::vector<bool> > > RingMeshMap;

};

#endif /* MESH_H_INCLUDED */

