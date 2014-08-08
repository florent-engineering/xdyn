/*
 * environment_parsersTest.cpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#include <sstream>

#include "environment_parsersTest.hpp"
#include "environment_parsers.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#define DEG (180./PI)

const YamlWaveModel environment_parsersTest::yaml = parse_waves(test_data::waves());

environment_parsersTest::environment_parsersTest() : a(DataGenerator(1212))
{
}

environment_parsersTest::~environment_parsersTest()
{
}

void environment_parsersTest::SetUp()
{
}

void environment_parsersTest::TearDown()
{
}

TEST_F(environment_parsersTest, default_wave_model)
{
//! [environment_parsersTest example]
    const double zwave = parse_default_wave_model("constant sea elevation in NED frame: {value: 1234, unit: m}");
//! [environment_parsersTest example]
//! [environment_parsersTest expected output]
    ASSERT_DOUBLE_EQ(1234,zwave);
//! [environment_parsersTest expected output]
}

TEST_F(environment_parsersTest, can_parse_wave_discretization)
{
    ASSERT_EQ(128,yaml.discretization.n);
    ASSERT_DOUBLE_EQ(0.1,yaml.discretization.omega_min);
    ASSERT_DOUBLE_EQ(6./DEG,yaml.discretization.omega_max);
}

TEST_F(environment_parsersTest, can_parse_wave_spreading_and_spectral_data)
{
    ASSERT_EQ(2,yaml.spectra.size());

    ASSERT_EQ("dirac",yaml.spectra.at(0).directional_spreading_type);
    ASSERT_EQ("type: dirac\nwaves coming from:\n  unit: deg\n  value: 90",yaml.spectra.at(0).directional_spreading_yaml);
    ASSERT_EQ("airy",yaml.spectra.at(0).model);
    ASSERT_EQ("directional spreading:\n  type: dirac\n  waves coming from:\n    unit: deg\n    value: 90\nmodel: airy\nseed of the random data generator: 0\nspectral density:\n  Hs:\n    unit: m\n    value: 5\n  Tp:\n    unit: m\n    value: 15\n  gamma: 1.2\n  type: jonswap",yaml.spectra.at(0).model_yaml);
    ASSERT_EQ("jonswap",yaml.spectra.at(0).spectral_density_type);
    ASSERT_EQ("Hs:\n  unit: m\n  value: 5\nTp:\n  unit: m\n  value: 15\ngamma: 1.2\ntype: jonswap",yaml.spectra.at(0).spectral_density_yaml);

    ASSERT_EQ("cos2s",yaml.spectra.at(1).directional_spreading_type);
    ASSERT_EQ("s: 2\ntype: cos2s\nwaves coming from:\n  unit: deg\n  value: 90",yaml.spectra.at(1).directional_spreading_yaml);
    ASSERT_EQ("airy",yaml.spectra.at(1).model);
    ASSERT_EQ("directional spreading:\n  s: 2\n  type: cos2s\n  waves coming from:\n    unit: deg\n    value: 90\nmodel: airy\nseed of the random data generator: 1872\nspectral density:\n  Hs:\n    unit: m\n    value: 5\n  Tp:\n    unit: m\n    value: 15\n  type: dirac",yaml.spectra.at(1).model_yaml);
    ASSERT_EQ("dirac",yaml.spectra.at(1).spectral_density_type);
    ASSERT_EQ("Hs:\n  unit: m\n  value: 5\nTp:\n  unit: m\n  value: 15\ntype: dirac",yaml.spectra.at(1).spectral_density_yaml);
}

TEST_F(environment_parsersTest, can_parse_wave_outputs)
{
    ASSERT_EQ("yaml",yaml.output.format);
    ASSERT_EQ("ball",yaml.output.frame_of_reference);
    ASSERT_EQ("waves_out.yaml",yaml.output.full_filename);
    ASSERT_DOUBLE_EQ(9,yaml.output.x.size());
    ASSERT_DOUBLE_EQ(1E3,yaml.output.x.at(0));
    ASSERT_DOUBLE_EQ(2E3,yaml.output.x.at(1));
    ASSERT_DOUBLE_EQ(3E3,yaml.output.x.at(2));
    ASSERT_DOUBLE_EQ(4E3,yaml.output.x.at(3));
    ASSERT_DOUBLE_EQ(5E3,yaml.output.x.at(4));
    ASSERT_DOUBLE_EQ(1E3,yaml.output.x.at(5));
    ASSERT_DOUBLE_EQ(2E3,yaml.output.x.at(6));
    ASSERT_DOUBLE_EQ(3E3,yaml.output.x.at(7));
    ASSERT_DOUBLE_EQ(4E3,yaml.output.x.at(8));
    ASSERT_EQ(9,yaml.output.y.size());
    ASSERT_DOUBLE_EQ(1E3,yaml.output.y.at(0));
    ASSERT_DOUBLE_EQ(1E3,yaml.output.y.at(1));
    ASSERT_DOUBLE_EQ(1E3,yaml.output.y.at(2));
    ASSERT_DOUBLE_EQ(1E3,yaml.output.y.at(3));
    ASSERT_DOUBLE_EQ(1E3,yaml.output.y.at(4));
    ASSERT_DOUBLE_EQ(2E3,yaml.output.y.at(5));
    ASSERT_DOUBLE_EQ(2E3,yaml.output.y.at(6));
    ASSERT_DOUBLE_EQ(2E3,yaml.output.y.at(7));
    ASSERT_DOUBLE_EQ(2E3,yaml.output.y.at(8));
}


TEST_F(environment_parsersTest, can_parse_jonswap_spectrum)
{
    const YamlJonswap y = parse_jonswap("Hs: {value: 5, unit: m}\n"
                                        "Tp: {value: 15, unit: m}\n"
                                        "gamma: 1.2\n");
    ASSERT_DOUBLE_EQ(5, y.Hs);
    ASSERT_DOUBLE_EQ(15, y.Tp);
    ASSERT_DOUBLE_EQ(1.2, y.gamma);
}

TEST_F(environment_parsersTest, DISABLED_can_parse_dirac_spectrum)
{
    ASSERT_TRUE(false);
}

TEST_F(environment_parsersTest, DISABLED_can_parse_pierson_moskowitz_spectrum)
{
    ASSERT_TRUE(false);
}

TEST_F(environment_parsersTest, DISABLED_can_parse_bretschneider_spectrum)
{
    ASSERT_TRUE(false);
}

TEST_F(environment_parsersTest, DISABLED_can_parse_dirac_spreading)
{
    ASSERT_TRUE(false);
}

TEST_F(environment_parsersTest, DISABLED_can_parse_cos2s_spreading)
{
    ASSERT_TRUE(false);
}

TEST_F(environment_parsersTest, DISABLED_airy_wave_model)
{
    std::stringstream yaml;
    yaml << "discretization:"                                  << std::endl
         << "   n: 128"                                        << std::endl
         << "   omega min: {value: 0.1, unit: rad/s}"          << std::endl
         << "   omega max: {value: 6, unit: rad/s}"            << std::endl
         << "spectra:"                                         << std::endl
         << "  - model: airy"                                  << std::endl
         << "    directional spreading:"                       << std::endl
         << "       type: dirac"                               << std::endl
         << "       waves coming from: {value: 90, unit: deg}" << std::endl
         << "    spectral density:"                            << std::endl
         << "       type: jonswap"                             << std::endl
         << "       Hs: {value: 5, unit: m}"                   << std::endl
         << "       Tp: {value: 15, unit: m}"                  << std::endl
         << "       gamma: 1.2"                                << std::endl
         << "  - model: airy"                                  << std::endl
         << "    directional spreading:"                       << std::endl
         << "       type: cos2s"                               << std::endl
         << "       s: 2"                                      << std::endl
         << "       waves coming from: {value: 90, unit: deg}" << std::endl
         << "    spectral density:"                            << std::endl
         << "       type: dirac"                               << std::endl
         << "       Hs: {value: 5, unit: m}"                   << std::endl
         << "       Tp: {value: 15, unit: m}"                  << std::endl
         << "output:"                                          << std::endl
         << "   format: yaml"                                  << std::endl
         << "   frame of reference: ball"                      << std::endl
         << "   mesh:"                                         << std::endl
         << "      unit: m"                                    << std::endl
         << "      x: [1,2,3,4,5,1,2,3,4]"                     << std::endl
         << "      x: [1,1,1,1,1,2,2,2,2]"                     << std::endl;
}
