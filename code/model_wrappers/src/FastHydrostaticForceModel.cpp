/*
 * HydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "hydrostatic.hpp"

FastHydrostaticForceModel::FastHydrostaticForceModel(const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(env_)
{
}

SurfaceForceModel::DF FastHydrostaticForceModel::dF(const FacetIterator& that_facet, const EnvironmentAndFrames& env, const Body& body, const double) const
{
    const double zG = hydrostatic::average_immersion(that_facet->vertex_index, body.intersector->all_immersions);
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const EPoint C = get_application_point(that_facet, body, zG);
    return DF(-env.rho*env.g*zG*dS,C);
}

EPoint FastHydrostaticForceModel::get_application_point(const FacetIterator& that_facet, const Body&, const double) const
{
    return that_facet->barycenter;
}