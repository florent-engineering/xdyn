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
    return YamlRotation();
}

double decode(const UV& uv)
{
    return uv.value * DecodeUnit::decodeUnit(uv.unit);
}
