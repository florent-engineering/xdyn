/*
 * ForceModel.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef FORCEMODEL_HPP_
#define FORCEMODEL_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <vector>
#include <ssc/kinematics.hpp>

class Body;
class EnvironmentAndFrames;

class ForceModel
{
    public:
        virtual ~ForceModel(){}
        virtual ssc::kinematics::Wrench operator()(const Body& body, const double t) const = 0;
        virtual double potential_energy(const Body& body, const std::vector<double>& x) const {(void)body;(void)x;return 0;}
};

typedef TR1(shared_ptr)<ForceModel> ForcePtr;
typedef std::vector<ForcePtr> ListOfForces;

#endif /* FORCEMODEL_HPP_ */
