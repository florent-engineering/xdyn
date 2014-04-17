/*
 * SimulatorYamlParser.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "SimulatorYamlParser.hpp"
#include "yaml.h"
#include "DecodeUnit.h"
#include "SimulatorYamlParserException.hpp"

struct UV
{
    UV() : value(0), unit("") {}
    double value;
    std::string unit;
};

void operator >> (const YAML::Node& node, YamlRotation& g);
void operator >> (const YAML::Node& node, UV& g);
void operator >> (const YAML::Node& node, YamlBody& b);
void operator >> (const YAML::Node& node, YamlModel& m);
void operator >> (const YAML::Node& node, YamlPosition& m);
void operator >> (const YAML::Node& node, YamlAngle& a);
void operator >> (const YAML::Node& node, YamlCoordinates& c);
void operator >> (const YAML::Node& node, YamlSpeed& s);
void operator >> (const YAML::Node& node, YamlDynamics& d);
void operator >> (const YAML::Node& node, YamlPoint& p);
void parse_point_with_name(const YAML::Node& node, YamlPoint& p, const std::string& name);
void parse_uv(const YAML::Node& node, double& d);
void operator >> (const YAML::Node& node, YamlInertiaMatrix& m);
void operator >> (const YAML::Node& node, YamlBlockedDegreesOfFreedom& m);
void operator >> (const YAML::Node& node, YamlOutput& p);
void operator >> (const YAML::Node& node, YamlPositionOutput& p);
void operator >> (const YAML::Node& node, YamlAnglesOutput& p);

double decode(const UV& uv);

SimulatorYamlParser::SimulatorYamlParser(const std::string& data) : YamlParser(data), node(new YAML::Node())
{
    convert_stream_to_yaml_node(contents, *node);
    if (node->size() == 0)
    {
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Something is wrong with the YAML data: no YAML nodes were detected by the YAML parser.");
    }
}

YamlSimulatorInput SimulatorYamlParser::parse()
{
    YamlSimulatorInput ret;
    (*node)["bodies"] >> ret.bodies;
    (*node)["rotations"] >> ret.rotations;
    (*node)["environment"] >> ret.environment;
    (*node)["points"] >> ret.points;
    (*node)["blocked degrees of freedom body/NED->body"] >> ret.blocked_degrees_of_freedom;
    (*node)["outputs"]["positions"] >> ret.position_output;
    (*node)["outputs"]["angles"] >> ret.angles_output;
    (*node)["outputs"]["linear velocities"] >> ret.linear_velocities_output;
    (*node)["outputs"]["angular velocities"] >> ret.angular_velocities_output;
    (*node)["outputs"]["linear accelerations"] >> ret.linear_accelerations_output;
    return ret;
}

void operator >> (const YAML::Node& node, YamlRotation& g)
{
    node["convention"] >> g.convention;
    node["order by"] >> g.order_by;
}

void operator >> (const YAML::Node& node, UV& g)
{
    node["unit"] >> g.unit;
    node["value"] >> g.value;
}

double decode(const UV& uv)
{
    return uv.value * DecodeUnit::decodeUnit(uv.unit);
}

void operator >> (const YAML::Node& node, YamlBody& b)
{
    node["name"] >> b.name;
    node["mesh"] >> b.mesh;
    node["external forces"] >> b.external_forces;
    node["position of body frame relative to mesh"] >> b.position_of_body_frame_relative_to_mesh;
    node["initial position of body frame relative to NED"] >> b.initial_position_of_body_frame_relative_to_NED_projected_in_NED;
    node["initial velocity of body frame relative to NED"] >> b.initial_velocity_of_body_frame_relative_to_NED_projected_in_body;
    node["dynamics"] >> b.dynamics;
}

void operator >> (const YAML::Node& node, YamlModel& m)
{
    node["model"] >> m.model;
    YAML::Emitter out;
    out << node;
    m.yaml = out.c_str();
}

void operator >> (const YAML::Node& node, YamlPosition& p)
{
    node >> p.angle;
    node >> p.coordinates;
    node["frame"] >> p.frame;
}

void operator >> (const YAML::Node& node, YamlAngle& a)
{
    parse_uv(node["phi"], a.phi);
    parse_uv(node["theta"], a.theta);
    parse_uv(node["psi"], a.psi);
}

void operator >> (const YAML::Node& node, YamlCoordinates& c)
{
    parse_uv(node["x"], c.x);
    parse_uv(node["y"], c.y);
    parse_uv(node["z"], c.z);
}

void operator >> (const YAML::Node& node, YamlSpeed& s)
{
    node["frame"] >> s.frame;
    parse_uv(node["u"], s.u);
    parse_uv(node["v"], s.v);
    parse_uv(node["w"], s.w);
    parse_uv(node["p"], s.p);
    parse_uv(node["q"], s.q);
    parse_uv(node["r"], s.r);
}

void operator >> (const YAML::Node& node, YamlDynamics& d)
{
    parse_point_with_name(node["centre of inertia"], d.centre_of_inertia, "centre of inertia");
    parse_uv(node["mass"], d.mass);
    node["inertia matrix divided by mass"] >> d.inertia_matrix_divided_by_mass;
}

void operator >> (const YAML::Node& node, YamlPoint& p)
{
    std::string name;
    node["name"] >> name;
    parse_point_with_name(node, p,name);
}

void parse_point_with_name(const YAML::Node& node, YamlPoint& p, const std::string& name)
{
    YamlCoordinates c;
    node >> c;
    p = c;
    node["frame"] >> p.frame;
    p.name = name;
}

void parse_uv(const YAML::Node& node, double& d)
{
    UV uv;
    node >> uv;
    d = decode(uv);
}

void operator >> (const YAML::Node& node, YamlInertiaMatrix& m)
{
    node["frame"] >> m.frame;
    node["row 1"] >> m.row_1;
    node["row 2"] >> m.row_2;
    node["row 3"] >> m.row_3;
    node["row 4"] >> m.row_4;
    node["row 5"] >> m.row_5;
    node["row 6"] >> m.row_6;
}

void operator >> (const YAML::Node& node, YamlBlockedDegreesOfFreedom& m)
{
    node["body"] >> m.body;
    node["blocked"] >> m.blocked;
}

void operator >> (const YAML::Node& node, YamlOutput& p)
{
    node["axes"] >> p.axes;
    node["projected in frame"] >> p.projected_in_frame;
}

void operator >> (const YAML::Node& node, YamlPositionOutput& p)
{
    YamlOutput o;
    node >> o;
    p = o;
    node["point"] >> p.point;
    node["relative to frame"] >> p.relative_to_frame;
}

void operator >> (const YAML::Node& node, YamlAnglesOutput& p)
{
    YamlOutput o;
    node >> o;
    p = o;
    node["frame"] >> p.frame;
    node["relative to frame"] >> p.relative_to_frame;
}
