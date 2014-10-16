/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"
#include "MeshIntersector.hpp"
#include "mesh_manipulations.hpp"
#include <ssc/macros.hpp>
#include "STL_data.hpp"
#include "StlReader.hpp"

#define EPS 1E-6

hydrostaticTest::hydrostaticTest() : a(ssc::random_data_generator::DataGenerator(2))
{
}

hydrostaticTest::~hydrostaticTest()
{
}

void hydrostaticTest::SetUp()
{
}

void hydrostaticTest::TearDown()
{
}

Matrix3x hydrostaticTest::get_rectangle(const double w, const double h) const
{
    Matrix3x rectangle_vertices(3,4);
    rectangle_vertices <<
            -w/2 , +w/2 , +w/2 , -w/2 ,
            -h/2 , -h/2 , +h/2 , +h/2 ,
             0   ,  0   ,  0   ,  0   ;
    return rectangle_vertices;
}

using namespace hydrostatic;

TEST_F(hydrostaticTest, can_compute_average_immersion)
{
//! [hydrostaticTest average_immersion_example]
    const Mesh mesh = MeshBuilder(trapezium()).build();
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double h = a.random<double>();
        ASSERT_DOUBLE_EQ(h/2., average_immersion(mesh.facets.front().vertex_index, {0,0,h,h}));
    }
//! [hydrostaticTest average_immersion_example]
}

TEST_F(hydrostaticTest, can_compute_average_immersion_even_when_all_nodes_are_used)
{
    const Mesh mesh = MeshBuilder(one_triangle()).build();
    ASSERT_DOUBLE_EQ(5, average_immersion(std::vector<double>({4,5,6})));
}

TEST_F(hydrostaticTest, can_compute_the_elementary_hydrostatic_force)
{
    const std::string frame = a.random<std::string>();
    const ssc::kinematics::Point O(frame, 1,2,4);
    const Eigen::Vector3d C(78,-4,6);
    const Eigen::Vector3d dS(0,3,4);
    const double rho = 1024;
    const double g = 10;
    const double dz = 3;
    const ssc::kinematics::UnsafeWrench Fhs = dF(O, C, rho, g, dz, dS);

    ASSERT_EQ(frame, Fhs.get_frame());

    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(-1024*10*3*3, Fhs.Y());
    ASSERT_DOUBLE_EQ(-1024*10*3*4, Fhs.Z());
    ASSERT_DOUBLE_EQ(-6*Fhs.Z()-2*Fhs.Y(), Fhs.K());
    ASSERT_DOUBLE_EQ(-77*Fhs.Z(), Fhs.M());
    ASSERT_DOUBLE_EQ(77*Fhs.Y(), Fhs.N());
}


TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_two_triangles)
{
    const MeshPtr mesh(new Mesh(MeshBuilder(two_triangles()).build()));
    const double rho = 1024;
    const EPoint g(0, 0, 10);
    const ssc::kinematics::Point G(a.random<std::string>(), 1,2,4);
    const std::vector<double> z = {-0.5,-0.5,-2.5,0.5};
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(z);
    const ssc::kinematics::Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);

    ASSERT_EQ(1,intersector.index_of_immersed_facets.size());
    ASSERT_DOUBLE_EQ(0.5,::area(intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0))));
    ASSERT_DOUBLE_EQ(0.5,mesh->facets[intersector.index_of_immersed_facets.at(0)].area);
    ASSERT_DOUBLE_EQ(0.5/3,hydrostatic::average_immersion(mesh->facets[intersector.index_of_immersed_facets.at(0)].vertex_index, intersector.all_relative_immersions));

    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    const double dz = 0.5/3;
    const double dS = 0.5;
    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(rho*10*dz*dS, Fhs.Z());
    ASSERT_DOUBLE_EQ(-8/3.*Fhs.Z(), Fhs.K());
    ASSERT_DOUBLE_EQ(-Fhs.Z(), Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_cube)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double L = a.random<double>().between(0,10);
        const double x0 = a.random<double>().between(-1000,1000);
        const double y0 = a.random<double>().between(-1000,1000);
        const double z0 = L/2;
        const ssc::kinematics::Point G(a.random<std::string>(), 0, 0, 0);
        const MeshPtr mesh(new Mesh(MeshBuilder(cube(L,x0,y0,z0)).build()));
        const std::vector<double> dz = {z0-L/2,z0-L/2,z0-L/2,z0-L/2,z0+L/2,z0+L/2,z0+L/2,z0+L/2};
        const double rho = 1000;
        const EPoint g(0, 0, 9.81);
        MeshIntersector intersector(mesh);
        intersector.update_intersection_with_free_surface(dz);
        const ssc::kinematics::Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L;
        ASSERT_SMALL_RELATIVE_ERROR(-rho*9.81*V, Fhs.Z(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(-rho*9.81*V*y0, Fhs.K(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(+rho*9.81*V*x0, Fhs.M(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.N(), EPS);
    }
}

TEST_F(hydrostaticTest, should_not_crash)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double L = a.random<double>().between(0,10);
        const double x0 = a.random<double>().between(-1000,1000);
        const double y0 = a.random<double>().between(-1000,1000);
        const double z0 = 0.0;
        const double h = 0.5*sqrt(2.0)*L;
        const ssc::kinematics::Point G(a.random<std::string>(), 0, 0, 0);
        const MeshPtr mesh(new Mesh(MeshBuilder(cube(L,x0,y0,z0)).build()));
        const std::vector<double> dz = {-h,0,0,-h,0,h,h,0};
        const double rho = 1000;
        const EPoint g(0, 0, 9.81);
        MeshIntersector intersector(mesh);
        intersector.update_intersection_with_free_surface(dz);
        force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
    }
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_stl_cube)
{
    const VectorOfVectorOfPoints mesh_cube(read_stl(test_data::cube()));
    const MeshPtr mesh(new Mesh(MeshBuilder(mesh_cube).build()));
    const std::vector<double> dz = {-1,-1,-1,-1,0,0,0,0};
    const ssc::kinematics::Point G(a.random<std::string>(), 0, 0, 0);
    const double rho = 1000;
    const EPoint g(0, 0, 9.81);
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(dz);
    const ssc::kinematics::Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_tetrahedron)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double L = a.random<double>().between(0,10);
        const double x0 = a.random<double>().between(-1000,1000);
        const double y0 = a.random<double>().between(-1000,1000);
        const double z0 = a.random<double>().between(0,1000);
        const ssc::kinematics::Point G(a.random<std::string>(), 0, 0, 0);
        const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
        const std::vector<double> dz = {z0, sqrt(6)*L/3 + z0, sqrt(6)*L/3 + z0, sqrt(6)*L/3 + z0};
        const double rho = 1000;
        const EPoint g(0, 0, 9.81);
        MeshIntersector intersector(mesh);
        intersector.update_intersection_with_free_surface(dz);
        const ssc::kinematics::Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L/(6.*sqrt(2));
        ASSERT_SMALL_RELATIVE_ERROR(-rho*9.81*V, Fhs.Z(), EPS);
    }
}

TEST_F(hydrostaticTest, hydrostatic_force_should_be_computed_at_the_right_point)
{
    const double L = a.random<double>().between(0,10);
    const double x0 = 1;
    const double y0 = 2;
    const double z0 = 3;
    const ssc::kinematics::Point G(a.random<std::string>(), a.random<double>(), a.random<double>(), a.random<double>());
    const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
    const std::vector<double> dz = a.random_vector_of<double>().of_size(4);
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(dz);
    const ssc::kinematics::Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, a.random<double>(), a.random<EPoint>());

    ASSERT_EQ(G.get_frame(), Fhs.get_point().get_frame());
    ASSERT_EQ(G.get_frame(), Fhs.get_frame());
    ASSERT_DOUBLE_EQ(G.x(), Fhs.get_point().x());
    ASSERT_DOUBLE_EQ(G.y(), Fhs.get_point().y());
    ASSERT_DOUBLE_EQ(G.z(), Fhs.get_point().z());
}

TEST_F(hydrostaticTest, inertia_of_rectangle)
{
    const double w = a.random<double>().between(-2,2);
    const double h = a.random<double>().between(-2,2);
    const auto all_nodes = get_rectangle(w,h);

    const std::vector<double> v = {1,2,3,4};

    const MeshPtr mesh(new Mesh(MeshBuilder(all_nodes).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);
    const auto Id=Eigen::MatrixXd::Identity(3,3);
    const auto J0=hydrostatic::get_inertia_of_polygon_wrt(intersector.begin_immersed() , Id , mesh->all_nodes );

    double Jx = h*std::pow(w,3) / 12.0;
    double Jy = w*std::pow(h,3) / 12.0;
    double area = w*h;
    Eigen::Matrix3d expected;
    expected <<
             Jx ,  0 ,  0  ,
             0  , Jy ,  0  ,
             0  ,  0 ,  0  ;
    ASSERT_LT((expected/area - J0).array().abs().maxCoeff(),1.0e-10);
}

TEST_F(hydrostaticTest, inertia_of_rotated_rectangle)
{
    const double w = a.random<double>().between(-2,2);
    const double h = a.random<double>().between(-2,2);
    const auto all_nodes = get_rectangle(w,h);

    const std::vector<double> v = {1,2,3,4};

    const MeshPtr mesh(new Mesh(MeshBuilder(all_nodes).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

    const double theta = a.random<double>().between(0,2*std::acos(1.0));
    const double c = std::cos(theta);
    const double s = std::sin(theta);
    Eigen::Matrix3d R10;
    R10 <<
             c , s , 0 ,
            -s , c , 0 ,
             0 , 0 , 1 ;
    const auto R01 = R10.transpose();

    const auto Id=Eigen::MatrixXd::Identity(3,3);
    const auto J0=hydrostatic::get_inertia_of_polygon_wrt(intersector.begin_immersed() , Id  , mesh->all_nodes );
    const auto J1=hydrostatic::get_inertia_of_polygon_wrt(intersector.begin_immersed() , R10 , mesh->all_nodes );

    const auto expected = R10*J0*R01;
    ASSERT_LT((expected - J1).array().abs().maxCoeff(),1.0e-10);
}
