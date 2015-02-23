/*
 * ResultantForceComputer.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "GZTypes.hpp"
#include "ResultantForceComputer.hpp"
#include "Sim.hpp"

GZ::ResultantForceComputer::ResultantForceComputer(const Sim& s) :
    body(s.get_bodies().front()),
    env(s.get_env()),
    gravity(s.get_forces().begin()->second.front()),
    hydrostatic(s.get_forces().begin()->second.back()),
    current_instant(0)
{

}

GZ::State GZ::ResultantForceComputer::operator()(const ::GZ::State& point)
{
    std::vector<double> x(13, 0);
    x[ZIDX(0)] = point.z;
    ssc::kinematics::EulerAngles angle(point.phi,point.theta,0);
    YamlRotation c;
    c.order_by = "angle";
    c.convention.push_back("z");
    c.convention.push_back("y'");
    c.convention.push_back("x''");
    const auto quaternion = body->get_quaternions(angle, c);
    x[QRIDX(0)] = std::get<0>(quaternion);
    x[QIIDX(0)] = std::get<1>(quaternion);
    x[QJIDX(0)] = std::get<2>(quaternion);
    x[QKIDX(0)] = std::get<3>(quaternion);
    body->update(env,x,current_instant);

    auto sum_of_forces = gravity->operator ()(body->get_states(),current_instant)
                       + hydrostatic->operator ()(body->get_states(),current_instant);

    current_instant += 1;
    return GZ::State(sum_of_forces.Z(), sum_of_forces.K(), sum_of_forces.M());
}

GZ::ResultantForceComputer::~ResultantForceComputer()
{
}