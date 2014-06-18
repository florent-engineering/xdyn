/*
 * SimulatorBuilder.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Kinematics.hpp"
#include "SimulatorBuilder.hpp"
#include "SimulatorBuilderException.hpp"
#include "update_kinematics.hpp"

SimulatorBuilder::SimulatorBuilder(const YamlSimulatorInput& input_) :
                                        input(input_),
                                        builder(BodyBuilder(input.rotations)),
                                        force_parsers(std::vector<ForceBuilderPtr>()),
                                        wave_parsers(std::vector<WaveBuilderPtr>())
{
}

std::vector<Body> SimulatorBuilder::get_bodies(const MeshMap& meshes) const
{
    std::vector<Body> ret;
    for (const auto body:input.bodies)
    {
        const auto that_mesh = meshes.find(body.name);
        if (that_mesh != meshes.end())
        {
            ret.push_back(builder.build(body, that_mesh->second));
        }
        else
        {
            THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, std::string("Unable to find mesh for '") + body.name + "' in map");
        }
    }
    return ret;
}

EnvironmentAndFrames SimulatorBuilder::get_environment_and_frames(const std::vector<Body>& bodies) const
{
    EnvironmentAndFrames env;
    env.g = input.environmental_constants.g;
    env.rho = input.environmental_constants.rho;
    env.k = KinematicsPtr(new Kinematics());
    const StateType x = get_initial_states(input.rotations, input.bodies);
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        env.k->add(get_transform_from_mesh_to(bodies.at(i)));
        env.k->add(get_transform_from_ned_to(x, bodies.at(i), i));
    }
    env.w = get_wave();
    return env;
}

WavePtr SimulatorBuilder::get_wave() const
{
    if (wave_parsers.empty())
    {
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, "No wave parser defined: use SimulatorBuilder::can_parse<T> with e.g. T=DefaultWaveModel");
    }
    WavePtr ret;
    for (const auto model:input.environment)
    {
        for (const auto parser:wave_parsers)
        {
            boost::optional<WavePtr> w = parser->try_to_parse(model.model, model.yaml);
            if (w)
            {
                if (ret.use_count())
                {
                    THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, "More than one wave model was defined.");
                }
                ret = w.get();
            }
        }
    }
    return ret;
}

std::vector<ListOfForces> SimulatorBuilder::get_forces(const EnvironmentAndFrames& env) const
{
    std::vector<ListOfForces> forces;
    for (const auto body:input.bodies)
    {
        forces.push_back(forces_from(body, env));
    }
    return forces;
}

ListOfForces SimulatorBuilder::forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const
{
    ListOfForces ret;
    for (auto force_model:body.external_forces) add(force_model, ret, env);
    return ret;
}

void SimulatorBuilder::add(const YamlModel& model, ListOfForces& L, const EnvironmentAndFrames& env) const
{
    bool parsed = false;
    for (auto parser:force_parsers)
    {
        boost::optional<ForcePtr> f = parser->try_to_parse(model.model, model.yaml, env);
        if (f)
        {
            L.push_back(f.get());
            parsed = true;
        }
    }
    if (not(parsed))
    {
        std::stringstream ss;
        ss << "Unable to find a parser to parse model '" << model.model;
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, ss.str());
    }
}
