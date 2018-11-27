/*
 * HOSTest.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: cady
 */

#include "environment_parsers.hpp"
#include "HOSTest.hpp"
#include "HOS.hpp"
#include "yaml_data.hpp"
#include <boost/algorithm/string.hpp> // replace in string

HOSTest::HOSTest() : a(ssc::random_data_generator::DataGenerator(892251200))
{
}

HOSTest::~HOSTest()
{
}

void HOSTest::SetUp()
{
}

void HOSTest::TearDown()
{
}

TEST_F(HOSTest, can_set_hos_parameters_LONG)
{
//! [HOSTest relative_wave_height example]
    auto hos = HOS::get_instance();
    hos.set_param(parse_hos(test_data::hos_for_parser_validation_only()));
//! [HOSTest relative_wave_height example]
//! [HOSTest relative_wave_height expected output]

//! [HOSTest relative_wave_height expected output]
}

TEST_F(HOSTest, relative_wave_height_LONG)
{
//! [HOSTest relative_wave_height example]
    auto hos = HOS::get_instance();
    hos.set_param(parse_hos(test_data::hos_for_parser_validation_only()));
//! [HOSTest relative_wave_height example]
//! [HOSTest relative_wave_height expected output]
    ASSERT_FLOAT_EQ(-0.39214659, hos.wave_height(0,0,0));
//! [HOSTest relative_wave_height expected output]
}

TEST_F(HOSTest, relative_wave_height_taking_direction_into_account_LONG)
{
    const double t = 0;//a.random<double>().between(0, 5);
    auto yaml = test_data::hos_for_parser_validation_only();
    auto hos = HOS::get_instance();

    boost::replace_all(yaml,"waves propagating to: {value: 90, unit: deg}","waves propagating to: {value: 0, unit: deg}");
    hos.set_param(parse_hos(yaml));
    const double hos_at_0_deg_1_0 = hos.wave_height(1,0,t);
    const double hos_at_0_deg_0_1 = hos.wave_height(0,1,t);
    const double hos_at_0_deg_minus_1_0 = hos.wave_height(-1,0,t);

    boost::replace_all(yaml,"waves propagating to: {value: 0, unit: deg}","waves propagating to: {value: 90, unit: deg}");
    hos.set_param(parse_hos(yaml));
    const double hos_at_90_deg_minus_1_0 = hos.wave_height(1,0,t);
    const double hos_at_90_deg_0_1 = hos.wave_height(0,1,t);

    boost::replace_all(yaml,"waves propagating to: {value: 90, unit: deg}","waves propagating to: {value: 180, unit: deg}");
    hos.set_param(parse_hos(yaml));
    const double hos_at_180_deg_1_0 = hos.wave_height(1,0,t);

    boost::replace_all(yaml,"waves propagating to: {value: 180, unit: deg}","waves propagating to: {value: 135, unit: deg}");
    hos.set_param(parse_hos(yaml));
    const double hos_at_135_deg_sqrt = hos.wave_height(-sqrt(2)/2,sqrt(2)/2,t);

    const double EPS=1E-2;
    ASSERT_NEAR(hos_at_0_deg_1_0, hos_at_90_deg_0_1, EPS);
    ASSERT_NEAR(hos_at_0_deg_0_1, hos_at_90_deg_minus_1_0, EPS);
    ASSERT_NEAR(hos_at_180_deg_1_0, hos_at_0_deg_minus_1_0, EPS);
    ASSERT_NEAR(hos_at_135_deg_sqrt, hos_at_0_deg_1_0, EPS);

}

TEST_F(HOSTest, dynamic_pressure_LONG)
{
//! [HOSTest dynamic_pressure example]
    const auto yaml = test_data::hos_for_parser_validation_only();
    auto hos = HOS::get_instance();
    hos.set_param(parse_hos(yaml));
    const double rho = 1024;
    const double g = 10;
    const double x = 1;
    const double y = 2;
    const double z = 3;
    const double eta = 4;
    const double t = 0.5;
//! [HOSTest dynamic_pressure example]
//! [HOSTest dynamic_pressure expected output]
    ASSERT_FLOAT_EQ(0.14270863, hos.dynamic_pressure(rho, g, x, y, z, eta, t));
//! [HOSTest dynamic_pressure expected output]
}

TEST_F(HOSTest, orbital_velocity_LONG)
{
    //! [HOSTest orbital_velocity example]
    const auto yaml = test_data::hos_for_parser_validation_only();
    auto hos = HOS::get_instance();
    hos.set_param(parse_hos(yaml));
    const double rho = 1024;
    const double x = 1;
    const double y = 2;
    const double z = 3;
    const double eta = 4;
    const double t = 0.5;
    //! [HOSTest orbital_velocity example]
    //! [HOSTest orbital_velocity expected output]
    const auto v = hos.orbital_velocity(rho, x, y, z, t, eta);
    ASSERT_FLOAT_EQ(0.10164127, (float)v.x());
    ASSERT_FLOAT_EQ(0.10164127, (float)v.y());
    ASSERT_FLOAT_EQ(-0.11665029, (float)v.z());
    //! [HOSTest orbital_velocity expected output]
}