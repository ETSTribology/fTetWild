//
// Created by Yixin Hu on 2019-08-27.
//

#ifndef FLOATTETWILD_TRIANGLEINSERTION_H
#define FLOATTETWILD_TRIANGLEINSERTION_H

#include <floattetwild/Mesh.hpp>
#include <floattetwild/AABBWrapper.h>
#include <floattetwild/CutMesh.h>

#ifdef FLOAT_TETWILD_USE_TBB
#include <tbb/concurrent_vector.h>
#endif

namespace floatTetWild {
    void insert_triangles(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                          const std::vector<int> &input_tags, Mesh &mesh,
                          std::vector<bool> &is_face_inserted, AABBWrapper &tree, bool is_again);

    void sort_input_faces(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                          const Mesh &mesh, std::vector<int> &sorted_f_ids);

    std::map<int, int> push_new_tets(Mesh &mesh, std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                                     std::vector<Vector3> &points, std::vector<MeshTet> &new_tets,
                                     std::vector<std::array<std::vector<int>, 4>> &new_track_surface_fs,
                                     std::vector<int> &modified_t_ids, bool is_again);

    ///face
    bool insert_one_triangle(int f_id, const std::vector<Vector3> &input_vertices,
                             const std::vector<Vector3i> &input_faces, const std::vector<int> &input_tags,
                             Mesh &mesh, std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                             AABBWrapper &tree, bool is_again);

    void
    find_cutting_tets(int f_id, const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                      const std::array<Vector3, 3> &vs, Mesh &mesh, std::vector<int> &result, bool is_again);

    bool subdivide_tets(int insert_f_id, Mesh &mesh, CutMesh &cut_mesh, std::vector<Vector3> &points,
                        std::map<std::array<int, 2>, int> &map_edge_to_intersecting_point,
                        std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                        std::vector<int> &subdivide_t_ids, std::vector<bool> &is_mark_surface,
                        std::vector<MeshTet> &new_tets,
                        std::vector<std::array<std::vector<int>, 4>> &new_track_surface_fs,
                        std::vector<int> &modified_t_ids,
                        std::vector<std::pair<int, int>>& new_covered_tet_fs,
                        bool is_track_covered_tet_fs = false);

    bool subdivide_tets_edge(int insert_f_id, Mesh &mesh, std::vector<Vector3> &points,
                             std::map<std::array<int, 2>, int> &map_edge_to_intersecting_point,
                             std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                             std::vector<int> &subdivide_t_ids,
                             std::vector<MeshTet> &new_tets,
                             std::vector<std::array<std::vector<int>, 4>> &new_track_surface_fs,
                             std::vector<int> &modified_t_ids,
                             std::vector<std::pair<int, int>>& new_covered_tet_fs);

    void pair_track_surface_fs(Mesh &mesh, std::vector<std::array<std::vector<int>, 4>> &track_surface_fs);

    ///edge
    void find_boundary_edges(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                             const std::vector<bool> &is_face_inserted,
                             std::vector<std::pair<std::array<int, 2>, std::vector<int>>> &b_edge_infos);

    bool insert_boundary_edges(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                               std::vector<std::pair<std::array<int, 2>, std::vector<int>>> &b_edge_infos,
                               std::vector<std::array<std::vector<int>, 4>> &track_surface_fs, Mesh &mesh,
                               AABBWrapper &tree, std::vector<std::array<int, 2>> &b_edges,
                               std::vector<bool> &is_face_inserted, bool is_again,
                               std::vector<std::array<int, 3>> &known_surface_fs,
                               std::vector<std::array<int, 3>> &known_not_surface_fs);

    bool insert_boundary_edges_get_intersecting_edges_and_points(
            const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
            const std::array<int, 2> &e, const std::vector<int> &n_f_ids,
            std::vector<std::array<std::vector<int>, 4>> &track_surface_fs, Mesh &mesh,
            std::vector<Vector3> &points, std::map<std::array<int, 2>, int> &map_edge_to_intersecting_point,
            std::vector<int> &snapped_v_ids, std::vector<std::array<int, 3>> &cut_fs,
            bool is_again);

    ///other
    void mark_surface_fs(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                         std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                         const std::vector<bool> &is_face_inserted,
                         const std::vector<std::array<int, 3>> &known_surface_fs,
                         const std::vector<std::array<int, 3>> &known_not_surface_fs,
                         std::vector<std::array<int, 2>> &b_edges,
                         Mesh &mesh, AABBWrapper &tree);
    void mark_boundary_vs(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                          std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                          const std::vector<std::pair<std::array<int, 2>, std::vector<int>>>& b_edge_infos,
                          Mesh &mesh, std::vector<std::array<int, 2>> &b_edges);

    int get_opp_t_id(int t_id, int j, const Mesh &mesh);

    inline int get_local_f_id(int t_id, int v1_id, int v2_id, int v3_id, Mesh &mesh) {
        for (int j = 0; j < 4; j++) {
            if (mesh.tets[t_id][j] != v1_id && mesh.tets[t_id][j] != v2_id && mesh.tets[t_id][j] != v3_id)
                return j;
        }
        assert(false);
        return -1;
    }

    void myassert(bool b, const std::string &s);

    void maintain_covered_region(const std::vector<std::pair<int, int>> &old_covered_tet_fs,
                                 const std::vector<int> &cut_t_ids,
                                 std::vector<std::pair<int, int>> &new_covered_tet_fs);

    bool preserve_edges(Mesh &mesh, std::vector<std::array<std::vector<int>, 4>> &track_surface_fs,
                        int insert_f_id, const std::array<Vector3, 3> &f_vs, const Vector3 &n, int t,
                        int e_id, std::vector<std::pair<int, int>> &covered_tet_fs);

    void check_track_surface_fs(Mesh &mesh, std::vector<std::array<std::vector<int>, 4>> &track_surface_fs);

    void check_is_input_face_covered(const std::array<Vector3, 3> f_vs,
                                     const Mesh &mesh, const std::vector<std::pair<int, int>> &t_fs);

    void check_edge_preservation(const std::vector<Vector3> &input_vertices, const std::vector<Vector3i> &input_faces,
                                 const std::vector<int> &sorted_f_ids, const Mesh &mesh,
                                 std::vector<std::array<std::vector<int>, 4>> &track_surface_fs);
}


#endif //FLOATTETWILD_TRIANGLEINSERTION_H
