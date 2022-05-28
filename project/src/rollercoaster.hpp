#pragma once


#include "cgp/cgp.hpp"
#include "racetrack.hpp"
#include "trajectoire.hpp"
#include "interpolation.h"
#include "elementar.hpp"

cgp::mesh_drawable create_rollercoaster_mesh_drawable();

class Wagon
{
    public:
        Wagon() = default;
        Wagon(Trajectoire _traj);
        cgp::hierarchy_mesh_drawable wagon;
        Trajectoire traj;

        cgp::hierarchy_mesh_drawable create_wagon_mesh_drawable();
        void faireAvancerWagon(float delta);
};


class Train{
    public:
        Train(int _nb_wagon, Trajectoire trajectoire);
        int nb_wagon;
        Wagon* train;
        void faireAvancerTrain();
};
