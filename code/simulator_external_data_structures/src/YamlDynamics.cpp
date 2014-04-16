/*
 * YamlDynamics.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlDynamics.hpp"

YamlDynamics::YamlDynamics() :
    position_of_centre_of_inertia(YamlPosition()),
    mass(0),
    inertia_matrix_divided_by_mass(YamlInertiaMatrix())
{

}
