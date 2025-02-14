// src/python_bindings.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <vector>
#include <Eigen/Dense>

// Include fTetWild headers
#include <floattetwild/Mesh.hpp>
#include <floattetwild/FloatTetDelaunay.h>

namespace py = pybind11;
using namespace floatTetWild;

// A simplified binding function: converts input vertices and faces (as Python lists)
// to the internal types, calls tetrahedralization, and returns a list of tetrahedra.
// (Note: In the actual fTetWild pipeline more parameters and setup are required.)
py::list tetrahedralize(const std::vector<std::vector<double>> &vertices,
                        const std::vector<std::vector<int>> &faces)
{
    // Convert Python list input into fTetWild-friendly vectors.
    std::vector<Eigen::Vector3d> verts;
    for (const auto &v : vertices)
    {
        if (v.size() != 3)
            throw std::runtime_error("Each vertex must have 3 coordinates.");
        verts.push_back(Eigen::Vector3d(v[0], v[1], v[2]));
    }
    
    std::vector<Eigen::Vector3i> fcs;
    for (const auto &f : faces)
    {
        if (f.size() != 3)
            throw std::runtime_error("Each face must have 3 indices.");
        fcs.push_back(Eigen::Vector3i(f[0], f[1], f[2]));
    }
    
    // Create a mesh instance to be populated.
    Mesh mesh;
    // For the purpose of this binding, we use a dummy vector for face insertion status.
    std::vector<bool> is_face_inserted(fcs.size(), false);

    // Note: The actual tetrahedralization call may be more complex because fTetWild
    // involves building acceleration structures (e.g., AABB trees) and several steps.
    // Here we assume a simplified interface for demonstration:
    FloatTetDelaunay::tetrahedralize(verts, fcs, mesh, is_face_inserted);

    // Convert the tetrahedra in mesh to a Python list.
    py::list tets;
    // We assume mesh.tets is a vector of a struct that has a member "indices"
    // containing 4 vertex indices. Adjust according to the actual data structure.
    for (const auto &tet : mesh.tets)
    {
        py::list tet_list;
        for (int idx : tet.indices) {
            tet_list.append(idx);
        }
        tets.append(tet_list);
    }
    return tets;
}

PYBIND11_MODULE(fTetWildPy, m) {
    m.doc() = "Python bindings for fTetWild: Fast Tetrahedral Meshing in the Wild";
    m.def("tetrahedralize", &tetrahedralize, "Generate a tetrahedral mesh from vertices and faces",
          py::arg("vertices"), py::arg("faces"));
}
