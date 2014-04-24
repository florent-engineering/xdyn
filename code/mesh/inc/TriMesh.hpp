#ifndef TRIMESH_HPP
#define TRIMESH_HPP

#include <vector>
#include "GeometricTypes3d.hpp"

/**
 * \author gj
 * \brief Contains a triangular mesh
 * \details
 * \ingroup mesh
 * \section ex1 Example
 * \snippet mesh/unit_tests/src/TriMeshTest.cpp TriMeshTest example
 * \section ex2 Expected output
 * \snippet mesh/unit_tests/src/TriMeshTest.cpp TriMeshTest expected output
 */
struct TriMesh
{
    TriMesh();
    TriMesh(const std::vector<Eigen::Vector3d>& nodes_, const std::vector<Facet>& facets_);
    std::vector<Eigen::Vector3d> nodes;
    std::vector<Facet> facets;
};

#endif //TRIMESH_HPP
