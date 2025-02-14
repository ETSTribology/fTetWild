#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <floattetwild/Mesh.hpp>
#include <floattetwild/FloatTetDelaunay.h>

namespace py = pybind11;

void tetrahedralize_mesh(std::vector<Eigen::Vector3d> &vertices,
                         std::vector<Eigen::Vector3i> &faces,
                         std::vector<Eigen::Vector3d> &tetrahedra) {
    floatTetWild::Mesh mesh;
    floatTetWild::FloatTetDelaunay::tetrahedralize(vertices, faces, mesh);
    tetrahedra = mesh.tets;
}

PYBIND11_MODULE(fTetWild, m) {
    m.doc() = "Python bindings for fTetWild"; // Module docstring

    m.def("tetrahedralize", &tetrahedralize_mesh, "Generate tetrahedral mesh");
}
