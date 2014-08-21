/*
 * mesh_manipulationsTest.cpp
 *
 *  Created on: May 20, 2014
 *      Author: cady
 */

#include "mesh_manipulationsTest.hpp"
#include "GeometricTypes3d.hpp"
#include "mesh_manipulations.hpp"
#include "StlReader.hpp"
#include "TriMeshTestData.hpp"

#include "test_macros.hpp"

mesh_manipulationsTest::mesh_manipulationsTest()
{
}

mesh_manipulationsTest::~mesh_manipulationsTest()
{
}

void mesh_manipulationsTest::SetUp()
{
}

void mesh_manipulationsTest::TearDown()
{
}

TEST_F(mesh_manipulationsTest, unit_normal)
{
//! [mesh_manipulationsTest unit_normal_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d n = unit_normal(M);
    ASSERT_DOUBLE_EQ(1, n.norm());
    ASSERT_DOUBLE_EQ(1/sqrt(3.), n(0));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), n(1));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), n(2));
//! [mesh_manipulationsTest unit_normal_example]
}

TEST_F(mesh_manipulationsTest, barycenter)
{
//! [mesh_manipulationsTest barycentre_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d G = barycenter(M);
    ASSERT_DOUBLE_EQ(1/3., G(0));
    ASSERT_DOUBLE_EQ(1/3., G(1));
    ASSERT_DOUBLE_EQ(1/3., G(2));
//! [mesh_manipulationsTest barycentre_example]
}

TEST_F(mesh_manipulationsTest, area)
{
//! [mesh_manipulationsTest area_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    ASSERT_DOUBLE_EQ(1, area(M));
//! [mesh_manipulationsTest area_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity)
{
//! [mesh_manipulationsTest centre_of_gravity_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 4; M(0,2) = 3; M(0,3) = 1;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(2,G(0));
    ASSERT_DOUBLE_EQ(4./9.,G(1));
    ASSERT_DOUBLE_EQ(0,G(2));
//! [mesh_manipulationsTest centre_of_gravity_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity_2)
{
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(0.5,G(0));
    ASSERT_DOUBLE_EQ(0.5,G(1));
    ASSERT_DOUBLE_EQ(0,G(2));
}

TEST_F(mesh_manipulationsTest, can_read_and_write_binary_stl)
{
    const VectorOfVectorOfPoints mesh = unit_cube();
    std::stringstream ss;

    write_binary_stl(mesh, ss);
    const VectorOfVectorOfPoints parsed_mesh = read_binary_stl(ss);

    ASSERT_EQ(mesh.size(), parsed_mesh.size());
    for (size_t i = 0 ; i < mesh.size() ; ++i)
    {
        ASSERT_EQ(mesh[i].size(), parsed_mesh[i].size()) << " (for i = " << i << ")";
        for (size_t j = 0 ; j < mesh[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)mesh[i][j](0), (double)parsed_mesh[i][j](0)) << " (for i = " << i << " and j = " << j << ", x-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](1), (double)parsed_mesh[i][j](1)) << " (for i = " << i << " and j = " << j << ", y-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](2), (double)parsed_mesh[i][j](2)) << " (for i = " << i << " and j = " << j << ", z-coordinate)";
        }
    }
}

