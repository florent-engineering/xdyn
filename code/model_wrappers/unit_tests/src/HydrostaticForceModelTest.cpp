/*
 * HydrostaticForceModuleTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "HydrostaticForceModel.hpp"
#include "HydrostaticForceModelTest.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"
#include "generate_body_for_tests.hpp"
#include "TriMeshTestData.hpp"
#include "Mesh.hpp"
#include "EnvironmentAndFrames.hpp"

#define BODY "body 1"
#include "test_macros.hpp"
HydrostaticForceModelTest::HydrostaticForceModelTest() : a(DataGenerator(212154))
{
}

HydrostaticForceModelTest::~HydrostaticForceModelTest()
{
}

void HydrostaticForceModelTest::SetUp()
{
}

void HydrostaticForceModelTest::TearDown()
{
}

TEST_F(HydrostaticForceModelTest, example)
{
    HydrostaticForceModel::Input input;
    input.g = 9.81;
    input.rho = 1024;
    input.k = KinematicsPtr(new Kinematics());
    const Point G("NED",0,2,2./3.);
    input.k->add(kinematics::Transform(Point("NED"), "mesh(" BODY ")"));
    input.k->add(kinematics::Transform(Point("NED"), BODY));
    input.w = WaveModelPtr(new DefaultSurfaceElevation(0));

    auto points = two_triangles();
    for (size_t i = 0 ; i < 2 ; ++i)
    {
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            double x = points[i][j][0];
            points[i][j][0] = points[i][j][2];
            points[i][j][2] = points[i][j][1];
            points[i][j][1] = x;
        }
    }

    Body body = get_body(BODY, points);
    body.G = G;

    HydrostaticForceModel F(input);
    const Wrench Fhs = F(body, a.random<double>());
//! [HydrostaticModuleTest example]
//! [HydrostaticModuleTest expected output]
    const double dz = 2./3;
    const double dS = 4;
    ASSERT_DOUBLE_EQ(input.rho*input.g*dz*dS, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(0, Fhs.Z());
    ASSERT_DOUBLE_EQ(0, Fhs.K());
    ASSERT_DOUBLE_EQ(0, Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}

/**
 * \note Test of a fully immerged rectangle plane with points [P1,P2,P3,P4]
 * P1 = [-2,+4,+6]
 * P2 = [-2,-4,+6]
 * P3 = [+2,+4,+4]
 * P4 = [+2,-4,+4]
 * The rectangle is divided into two triangles
 * T1 = [P1,P2,P3]
 * T2 = [P3,P2,P4]
 *
 * The triangles have the same area: \f$8\sqrt(5)\f$
 *
 * The triangles have the same unit normal:
 * \f$[\sin(\atan(0.5)),0,\cos(\atan(0.5))]\f$
 *
 * The immerged volume is 200m3, the norm of the resulting effort should be
 * \f$200*\rho*g\f$.
 *
 * The resulting force evaluated at the origin point O in the global frame (NED)
 * should have no component along axis Y
 *
 * The resulting torque evaluated at the origin point O in the global frame (NED)
 * should have no component around X and Z.
 */
TEST_F(HydrostaticForceModelTest, DISABLED_oriented_fully_immerged_rectangle)
{
    HydrostaticForceModel::Input input;
    input.g = 9.81;
    input.rho = 1024;
    input.k = KinematicsPtr(new Kinematics());
    const Point G("NED",0,0,0);
    input.k->add(kinematics::Transform(Point("NED"), "mesh(" BODY ")"));
    input.k->add(kinematics::Transform(Point("NED"), BODY));
    input.w = WaveModelPtr(new DefaultSurfaceElevation(0));

    const auto points = two_triangles_immerged();

    Body body = get_body(BODY, points);
    body.G = G;

    ASSERT_DOUBLE_EQ(0.0, body.x_relative_to_mesh);
    ASSERT_DOUBLE_EQ(0.0, body.y_relative_to_mesh);
    ASSERT_DOUBLE_EQ(0.0, body.z_relative_to_mesh);
    ASSERT_DOUBLE_EQ(1.0, body.mesh_to_body(0,0));
    ASSERT_DOUBLE_EQ(1.0, body.mesh_to_body(1,1));
    ASSERT_DOUBLE_EQ(1.0, body.mesh_to_body(2,2));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(0,1));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(0,2));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(1,0));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(1,2));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(2,0));
    ASSERT_DOUBLE_EQ(0.0, body.mesh_to_body(2,1));

    HydrostaticForceModel F(input);
    const Wrench Fhs = F(body,a.random<double>());

    ASSERT_EQ(3,(size_t)body.mesh->nodes.rows());
    ASSERT_EQ(4,(size_t)body.mesh->nodes.cols());

    ASSERT_DOUBLE_EQ(-2.0,body.mesh->nodes(0,0));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(1,0));
    ASSERT_DOUBLE_EQ(+6.0,body.mesh->nodes(2,0));

    ASSERT_DOUBLE_EQ(-2.0,body.mesh->nodes(0,1));
    ASSERT_DOUBLE_EQ(-4.0,body.mesh->nodes(1,1));
    ASSERT_DOUBLE_EQ(+6.0,body.mesh->nodes(2,1));

    ASSERT_DOUBLE_EQ(+2.0,body.mesh->nodes(0,2));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(1,2));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(2,2));

    ASSERT_DOUBLE_EQ(+2.0,body.mesh->nodes(0,3));
    ASSERT_DOUBLE_EQ(-4.0,body.mesh->nodes(1,3));
    ASSERT_DOUBLE_EQ(+4.0,body.mesh->nodes(2,3));

    ASSERT_EQ(2,body.mesh->facets.size());
    for (size_t i=0;i<2;++i)
    {
        ASSERT_DOUBLE_EQ(sin(atan(0.5)),body.mesh->facets[i].unit_normal(0));
        ASSERT_DOUBLE_EQ(0.0,body.mesh->facets[i].unit_normal(1));
        ASSERT_DOUBLE_EQ(cos(atan(0.5)),body.mesh->facets[i].unit_normal(2));
    }

    ASSERT_DOUBLE_EQ(-2.0/3.0,body.mesh->facets[0].barycenter(0));
    ASSERT_DOUBLE_EQ(+4.0/3.0,body.mesh->facets[0].barycenter(1));
    ASSERT_DOUBLE_EQ(5.0+1.0/3.0,body.mesh->facets[0].barycenter(2));

    ASSERT_DOUBLE_EQ(+2.0/3.0,body.mesh->facets[1].barycenter(0));
    ASSERT_DOUBLE_EQ(-4.0/3.0,body.mesh->facets[1].barycenter(1));
    ASSERT_DOUBLE_EQ(4.0+2.0/3.0,body.mesh->facets[1].barycenter(2));

    // What is currently implemented with the hypothesis of application
    // point force located at barycenter of each face
    ASSERT_DOUBLE_EQ(-1024*9.81*8*sqrt(5)*sin(atan(0.5))*(5+1.0/3.0+5-1/3.0), Fhs.X());
    ASSERT_DOUBLE_EQ(0.0, Fhs.Y());
    ASSERT_DOUBLE_EQ(-1024*9.81*8*sqrt(5)*cos(atan(0.5))*(5+1.0/3.0+5-1/3.0), Fhs.Z());

    // What is expected with the correct evaluation of application point force
    // All these tests fail.
    EXPECT_DOUBLE_EQ(input.rho*input.g*200.0, sqrt(Fhs.X()*Fhs.X()+Fhs.Z()*Fhs.Z()));
    ASSERT_DOUBLE_EQ(0.0, Fhs.Y());
    EXPECT_DOUBLE_EQ(-input.rho*input.g*200.0*sin(atan(0.5)), Fhs.X());
    EXPECT_DOUBLE_EQ(-input.rho*input.g*200.0*cos(atan(0.5)), Fhs.Z());
    ASSERT_DOUBLE_EQ(0, Fhs.K());
    ASSERT_DOUBLE_EQ(input.rho*input.g*200.0 * (5.0*sin(atan(0.5))+sqrt(5.0)/3.0), Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}