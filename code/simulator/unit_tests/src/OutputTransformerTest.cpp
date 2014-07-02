/*
 * OutputGeneratorTest.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputTransformerTest.hpp"
#include "OutputTransformer.hpp"
#include "yaml_data.hpp"
#include "simulator_api.hpp"
#include "steppers.hpp"
#include "SimulatorYamlParser.hpp"
#include "test_macros.hpp"
#define PI (4.*atan(1.))

const YamlSimulatorInput OutputTransformerTest::yaml1 = OutputTransformerTest::get_yaml1();
const YamlSimulatorInput OutputTransformerTest::yaml2 = OutputTransformerTest::get_yaml2();

YamlSimulatorInput OutputTransformerTest::get_yaml1()
{
    return SimulatorYamlParser(test_data::falling_ball_example()).parse();
}

YamlSimulatorInput OutputTransformerTest::get_yaml2()
{
    YamlSimulatorInput ret = SimulatorYamlParser(test_data::full_example()).parse();
    ret.bodies.front().mesh = "";
    return ret;
}

OutputTransformerTest::OutputTransformerTest() : a(DataGenerator(42022)), out1(std::vector<std::map<std::string,double> >()), out2(std::vector<std::map<std::string,double> >())
{
    auto res1 = simulate<EulerStepper>(yaml1, 0, 2, 1);
    const OutputTransformer transform1(yaml1);
    for (const auto r:res1) out1.push_back(transform1(r));
    auto res2 = simulate<EulerStepper>(yaml2, 0, 2, 1);
    const OutputTransformer transform2(yaml2);
    for (const auto r:res2) out2.push_back(transform2(r));
}

OutputTransformerTest::~OutputTransformerTest()
{
}

void OutputTransformerTest::SetUp()
{
}

void OutputTransformerTest::TearDown()
{
}

TEST_F(OutputTransformerTest, processed_output_should_have_the_right_size)
{
    ASSERT_EQ(3, out1.size());
    ASSERT_EQ(3, out2.size());
}

TEST_F(OutputTransformerTest, can_compute_positions)
{
    const auto x0 = out1.at(0)["x(O in NED / ball -> ball)"];
    const auto z0 = out1.at(0)["z(O in NED / ball -> ball)"];
    const auto x2 = out1.at(2)["x(O in NED / ball -> ball)"];
    const auto z2 = out1.at(2)["z(O in NED / ball -> ball)"];
    ASSERT_DOUBLE_EQ(-4,         x0) << "t = 0";
    ASSERT_DOUBLE_EQ(-12,        z0) << "t = 0";
    ASSERT_DOUBLE_EQ(-(4+2),     x2) << "t = 2";
    ASSERT_DOUBLE_EQ(-(12+9.81), z2) << "t = 2";
}
