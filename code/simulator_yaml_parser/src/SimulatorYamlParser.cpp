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
    double value;
    std::string unit;
};

void operator >> (const YAML::Node& node, YamlRotation& g);
void operator >> (const YAML::Node& node, UV& g);

double decode(const UV& uv);

SimulatorYamlParser::SimulatorYamlParser(const std::string& data) : YamlParser(data), node(new YAML::Node())
{
    convert_stream_to_yaml_node(contents, *node);
    if (node->size() == 0)
    {
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Something is wrong with the YAML data: no YAML nodes were detected by the YAML parser.");
    }
}

YamlRotation SimulatorYamlParser::get_rotation()
{
    YamlRotation ret;
    (*node)["rotations"] >> ret;
    return ret;
}

YamlModel SimulatorYamlParser::parse_model(const std::string& yaml) const
{
    YAML::Node n;
    convert_stream_to_yaml_node(yaml, n);
    YamlModel ret;
    n["type"] >> ret.type;
    YAML::Emitter out;
    out << n;
    ret.yaml = out.c_str();
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
