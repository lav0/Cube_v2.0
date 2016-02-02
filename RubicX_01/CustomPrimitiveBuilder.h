#pragma once


#include "CustomPrimitiveCubeInfo.h"
#include "CustomPrimitiveFunctions.h"

#include <vector>
#include <memory>
#include <algorithm>

template <typename VertexType, typename VectorType>
class CustomPrimitiveBuilder
{
  using Vertices = std::vector<VertexType>;
  using Indices = std::vector<uint16_t>;

public:

  CustomPrimitiveBuilder(
    size_t               tessellation,
    float                corner_rounding_coef,
    float                size,
    const CubeColorsMap& colors)
    : m_tessellation(tessellation)
    , m_corner_rounding_coef(corner_rounding_coef)
    , m_edge_rounding_coef(0.4f * corner_rounding_coef)
    , m_size(0.5f * size)
    , m_side_to_color_map(colors)
  {
    using namespace CustomPrimitiveCubeInfo;

    const short FACE_SIDE_VERTEX_COUNT = 12;
    const short EDGES_COUNT = 12;
    const short CORNERS_PER_FACE = 4;

    // calculate what size *m_vertices* and *m_indices* will have.
    size_t vertices_count =
      FACE_SIDE_VERTEX_COUNT * m_faceCount +
      m_faceCount * CORNERS_PER_FACE * (m_tessellation + 1) +
      EDGES_COUNT * 2 * (m_tessellation + 1) +
      24 * (m_tessellation + 1) * (m_tessellation + 1);

    size_t indices_count =
      m_faceCount * (4 * 9 + 6) +
      3 * (m_faceCount * CORNERS_PER_FACE * (m_tessellation - 1)) +
      EDGES_COUNT * 6 * m_tessellation +
      24 * 6 * m_tessellation * m_tessellation;

    m_vertices.reserve(vertices_count);
    m_indices.reserve(indices_count);
  }

  bool CreateFaces(const VectorType& centre)
  {
    using namespace CustomPrimitiveCubeInfo;
    using namespace CustomPrimitiveFunctions;

    // Create each face in turn.
    for (int i = 0; i < m_faceCount; i++)
    {
      VectorType normal = m_faceNormals[i];

      // Get two vectors perpendicular both to the face normal and to each other.
      VectorType basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;
      
      VectorType side1 = CrossProduct(normal, basis);
      VectorType side2 = CrossProduct(normal, side1);

      VectorType side1_main = VectorScale(
        side1, (1 - m_corner_rounding_coef) * (1 - m_edge_rounding_coef));

      VectorType side2_main = VectorScale(
        side2, (1 - m_corner_rounding_coef) * (1 - m_edge_rounding_coef));

      VectorType side1_aux = VectorScale(
        side1, m_corner_rounding_coef * (1 - m_edge_rounding_coef));

      VectorType side2_aux = VectorScale(
        side2, m_corner_rounding_coef * (1 - m_edge_rounding_coef));

      VectorType vertex0 = centre + (normal - side1_main - side2_main) * m_size;
      VectorType vertex1 = centre + (normal - side1_main + side2_main) * m_size;
      VectorType vertex2 = centre + (normal + side1_main + side2_main) * m_size;
      VectorType vertex3 = centre + (normal + side1_main - side2_main) * m_size;

      VectorType vertex00_aux = vertex0 - side2_aux * m_size;
      VectorType vertex01_aux = vertex0 - side1_aux * m_size;
      VectorType vertex10_aux = vertex1 - side1_aux * m_size;
      VectorType vertex11_aux = vertex1 + side2_aux * m_size;
      VectorType vertex20_aux = vertex2 + side2_aux * m_size;
      VectorType vertex21_aux = vertex2 + side1_aux * m_size;
      VectorType vertex30_aux = vertex3 + side1_aux * m_size;
      VectorType vertex31_aux = vertex3 - side2_aux * m_size;

      fillInIndicesContainer(m_indices, m_vertices.size());

      eCubeSide thisSide = facesMap.at(i);

      int textureShift = static_cast<int>(m_side_to_color_map.at(thisSide));

      // Four main vertices per face.
      m_vertices.emplace_back(vertex0, normal, m_textureCoordinates[textureShift + 0]);
      m_vertices.emplace_back(vertex1, normal, m_textureCoordinates[textureShift + 1]);
      m_vertices.emplace_back(vertex2, normal, m_textureCoordinates[textureShift + 2]);
      m_vertices.emplace_back(vertex3, normal, m_textureCoordinates[textureShift + 3]);

      // Extra eight (two in each corner) vertices per face
      m_vertices.emplace_back(vertex00_aux, normal, m_textureCoordinates[textureShift + 0]);
      m_vertices.emplace_back(vertex01_aux, normal, m_textureCoordinates[textureShift + 1]);
      m_vertices.emplace_back(vertex10_aux, normal, m_textureCoordinates[textureShift + 2]);
      m_vertices.emplace_back(vertex11_aux, normal, m_textureCoordinates[textureShift + 3]);
      m_vertices.emplace_back(vertex20_aux, normal, m_textureCoordinates[textureShift + 0]);
      m_vertices.emplace_back(vertex21_aux, normal, m_textureCoordinates[textureShift + 1]);
      m_vertices.emplace_back(vertex30_aux, normal, m_textureCoordinates[textureShift + 2]);
      m_vertices.emplace_back(vertex31_aux, normal, m_textureCoordinates[textureShift + 3]);

      // close quater-circle gaps in corners on each surface
      closeGaps(centre,
        m_faceNormals[i],
        m_faceNormals[i > 1 ? 0 : 2],
        m_textureCoordinates[textureShift + 0]);
    }

    return true;
  }

  bool CreateEdges(const VectorType& centre)
  {
    using namespace CustomPrimitiveCubeInfo;

    slickEdges(centre, m_faceNormals[0], m_faceNormals[2]);
    slickEdges(centre, m_faceNormals[0], m_faceNormals[3]);
    slickEdges(centre, m_faceNormals[0], m_faceNormals[4]);
    slickEdges(centre, m_faceNormals[0], m_faceNormals[5]);
    slickEdges(centre, m_faceNormals[1], m_faceNormals[2]);
    slickEdges(centre, m_faceNormals[1], m_faceNormals[3]);
    slickEdges(centre, m_faceNormals[1], m_faceNormals[4]);
    slickEdges(centre, m_faceNormals[1], m_faceNormals[5]);
    slickEdges(centre, m_faceNormals[2], m_faceNormals[4]);
    slickEdges(centre, m_faceNormals[2], m_faceNormals[5]);
    slickEdges(centre, m_faceNormals[3], m_faceNormals[4]);
    slickEdges(centre, m_faceNormals[3], m_faceNormals[5]);

    return true;
  }

  bool CreateCorners(const VectorType& centre)
  {
    using namespace CustomPrimitiveCubeInfo;

    slickCorners(centre, m_faceNormals[0], m_faceNormals[2]);
    slickCorners(centre, m_faceNormals[2], m_faceNormals[0]);
    slickCorners(centre, m_faceNormals[0], m_faceNormals[4]);
    slickCorners(centre, m_faceNormals[4], m_faceNormals[0]);
    slickCorners(centre, m_faceNormals[4], m_faceNormals[2]);
    slickCorners(centre, m_faceNormals[2], m_faceNormals[4]);

    slickCorners(centre, m_faceNormals[1], m_faceNormals[2]);
    slickCorners(centre, m_faceNormals[2], m_faceNormals[1]);
    slickCorners(centre, m_faceNormals[1], m_faceNormals[4]);
    slickCorners(centre, m_faceNormals[4], m_faceNormals[1]);

    slickCorners(centre, m_faceNormals[0], m_faceNormals[3]);
    slickCorners(centre, m_faceNormals[3], m_faceNormals[0]);
    slickCorners(centre, m_faceNormals[4], m_faceNormals[3]);
    slickCorners(centre, m_faceNormals[3], m_faceNormals[4]);

    slickCorners(centre, m_faceNormals[1], m_faceNormals[3]);
    slickCorners(centre, m_faceNormals[3], m_faceNormals[1]);

    slickCorners(centre, m_faceNormals[0], m_faceNormals[5]);
    slickCorners(centre, m_faceNormals[5], m_faceNormals[0]);
    slickCorners(centre, m_faceNormals[5], m_faceNormals[2]);
    slickCorners(centre, m_faceNormals[2], m_faceNormals[5]);

    slickCorners(centre, m_faceNormals[1], m_faceNormals[5]);
    slickCorners(centre, m_faceNormals[5], m_faceNormals[1]);
    slickCorners(centre, m_faceNormals[5], m_faceNormals[3]);
    slickCorners(centre, m_faceNormals[3], m_faceNormals[5]);

    return true;
  }

  Vertices& GetVertices() { return m_vertices; }
  Indices& GetIndices() { return m_indices; }

private:

  void closeGaps(const VectorType& centre, 
                 const VectorType& face_normal, 
                 const VectorType& normal1, 
                 const VectorType& texture)
  {
    using namespace CustomPrimitiveFunctions;

    assert(is_zero_dbl(GetX(DotProduct(face_normal, normal1))));

    VectorType axis = Normalize(face_normal);

    VectorType start = normal1;
    auto step = XM_PIDIV2;
    for (float travel = 0.f; travel < XM_2PI - 0.01f; travel += step)
    {
      VectorType end = RotateWithQuaternion(start, QuaternionRotationAxis(axis, -step));

      VectorType point = VectorScale(
        (start + end) * (1 - m_edge_rounding_coef) * (1 - m_corner_rounding_coef) + face_normal,
        m_size
      );

      closeGapInCorner(face_normal, end, start, centre + point, texture,
        m_size * m_corner_rounding_coef * (1 - m_edge_rounding_coef),
        m_tessellation,
        m_indices, m_vertices
      );

      start = end;
    }
  }
  
  void slickEdges(const VectorType& centre,
                  const VectorType& normal1,
                  const VectorType& normal2)
  {
    using namespace CustomPrimitiveCubeInfo;
    using namespace CustomPrimitiveFunctions;

    VectorType normal3 = CrossProduct(normal1, normal2);

    VectorType left_point = VectorScale(
      normal1 + normal2 - normal3*(1 - m_corner_rounding_coef),
      m_size*(1 - m_edge_rounding_coef)
    );
    VectorType right_point = VectorScale(
      normal1 + normal2 + normal3*(1 - m_corner_rounding_coef),
      m_size*(1 - m_edge_rounding_coef)
    );

    buildQuaterCylinderOnEdge(normal1, normal2, centre + left_point, centre + right_point,
      m_textureCoordinates[0], m_textureCoordinates[1],
      m_size * m_edge_rounding_coef, m_tessellation, m_indices, m_vertices);
  }
  
  void slickCorners(const VectorType& centre,
                    const VectorType& normal1,
                    const VectorType& normal2)
  {
    using namespace CustomPrimitiveCubeInfo;
    using namespace CustomPrimitiveFunctions;

    VectorType normal3 = CrossProduct(normal1, normal2);

    VectorType point = VectorScale(
      (normal1 + normal2)*(1 - m_corner_rounding_coef) + normal3,
      m_size*(1 - m_edge_rounding_coef)
      );

    buildTorusOnCorner(normal1, normal2, centre + point,
      m_textureCoordinates[0],
      m_size*m_edge_rounding_coef,
      m_size*(1 - m_edge_rounding_coef)*m_corner_rounding_coef,
      m_tessellation, m_indices, m_vertices);
  }

  static void closeGapInCorner(
    const VectorType& face_normal,
    const VectorType& normal_start,
    const VectorType& normal_end,
    const VectorType& corner_centre,
    const VectorType& texture_coord,
    float corner_radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<VertexType>& vertices
    )
  {
    using namespace CustomPrimitiveFunctions;

    VectorType axis = Normalize(face_normal);

    size_t vbase = vertices.size();

    auto full_angle = GetX(AngleBetweenNormals(normal_start, normal_end));

    for (size_t i = 0; i <= tessellation; ++i)
    {
      auto angle = i * full_angle / tessellation;

      VectorType rotation = QuaternionRotationAxis(axis, angle);
      VectorType normal = Normalize(RotateWithQuaternion(normal_start, rotation));

      VectorType point = corner_centre + corner_radius * normal;

      vertices.emplace_back(point, axis, texture_coord);

      if (i != 0 && i != 1) {
        indices.emplace_back(vbase);
        indices.emplace_back(vbase + i);
        indices.emplace_back(vbase + i - 1);
      }
    }
  }

  static void buildQuaterCylinderOnEdge(
    const VectorType& normal_1,
    const VectorType& normal_2,
    const VectorType& bottom_centre,
    const VectorType& top_centre,
    const VectorType& texture_coord1,
    const VectorType& texture_coord2,
    float radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<VertexType>& vertices
    )
  {
    using namespace CustomPrimitiveFunctions;

    size_t vbase = vertices.size();

    VectorType axis = Normalize(CrossProduct(normal_1, normal_2));

    for (size_t i = 0; i <= tessellation; ++i)
    {
      float angle = i * XM_PIDIV2 / tessellation;

      VectorType rotation = QuaternionRotationAxis(axis, angle);
      VectorType rad_vector = radius * RotateWithQuaternion(normal_1, rotation);

      VectorType point_bottom = bottom_centre + rad_vector;
      VectorType point_top = top_centre + rad_vector;

      vertices.emplace_back(point_bottom, rad_vector, texture_coord1);
      vertices.emplace_back(point_top, rad_vector, texture_coord2);

      if (i != tessellation)
      {
        indices.emplace_back(vbase + i * 2);
        indices.emplace_back(vbase + i * 2 + 1);
        indices.emplace_back(vbase + i * 2 + 2);

        indices.emplace_back(vbase + i * 2 + 1);
        indices.emplace_back(vbase + i * 2 + 3);
        indices.emplace_back(vbase + i * 2 + 2);
      }
    }
  }

  static void buildTorusOnCorner(
    const VectorType& normal_start,
    const VectorType& normal_end,
    const VectorType& torus_centre,
    const VectorType& texture_coord,
    float tube_radius,
    float torus_radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<VertexType>& vertices
    )
  {
    using namespace CustomPrimitiveFunctions;

    size_t vbase = vertices.size();
    size_t stride = tessellation + 1;

    VectorType axis = Normalize(CrossProduct(normal_start, normal_end));
    VectorType tangent = RotateWithQuaternion(
      normal_start,
      QuaternionRotationAxis(axis, -XM_PIDIV2)
    );

    auto section_angle = GetX(AngleBetweenNormals(normal_start, normal_end));

    for (size_t i = 0; i <= tessellation; ++i)
    {
      float torus_angle = i * section_angle / tessellation;

      VectorType rotation = QuaternionRotationAxis(axis, torus_angle);
      VectorType torus_normal = Normalize(RotateWithQuaternion(normal_start, rotation));
      VectorType torus_axis = Normalize(RotateWithQuaternion(tangent, rotation));

      for (size_t j = 0; j <= tessellation; ++j)
      {
        float tube_angle = j * XM_PIDIV2 / tessellation;

        VectorType tube_rotation = QuaternionRotationAxis(torus_axis, tube_angle);
        VectorType tube_normal = RotateWithQuaternion(torus_normal, tube_rotation);

        VectorType point = torus_centre + torus_radius * torus_normal + tube_radius * tube_normal;

        vertices.emplace_back(point, tube_normal, texture_coord);

        if (j != 0 && i != tessellation)
        {
          indices.emplace_back(vbase + i * stride + j);
          indices.emplace_back(vbase + (i + 1) * stride + (j - 1));
          indices.emplace_back(vbase + i * stride + (j - 1));

          indices.emplace_back(vbase + i * stride + j);
          indices.emplace_back(vbase + (i + 1) * stride + j);
          indices.emplace_back(vbase + (i + 1) * stride + (j - 1));
        }
      }
    }
  }

  static void fillInIndicesContainer(Indices& indices, const size_t& vbase)
  {
    //
    // central square
    //
    indices.emplace_back(vbase + 0);
    indices.emplace_back(vbase + 1);
    indices.emplace_back(vbase + 2);

    indices.emplace_back(vbase + 0);
    indices.emplace_back(vbase + 2);
    indices.emplace_back(vbase + 3);

    //
    //  triangle of vertex0, vertex00_aux and vertex01_aux 
    //  coner triangle
    indices.emplace_back(vbase + 0);
    indices.emplace_back(vbase + 4);
    indices.emplace_back(vbase + 5);
    //  side triangle  1 
    indices.emplace_back(vbase + 0);
    indices.emplace_back(vbase + 5);
    indices.emplace_back(vbase + 1);
    //  side triangle  2
    indices.emplace_back(vbase + 5);
    indices.emplace_back(vbase + 6);
    indices.emplace_back(vbase + 1);

    //
    //  triangle of vertex1, vertex10_aux and vertex11_aux
    //  coner triangle
    indices.emplace_back(vbase + 1);
    indices.emplace_back(vbase + 6);
    indices.emplace_back(vbase + 7);
    //  side triangle  1
    indices.emplace_back(vbase + 1);
    indices.emplace_back(vbase + 7);
    indices.emplace_back(vbase + 2);
    //  side triangle  2
    indices.emplace_back(vbase + 7);
    indices.emplace_back(vbase + 8);
    indices.emplace_back(vbase + 2);

    //
    //  triangle of vertex2, vertex20_aux and vertex21_aux
    //
    indices.emplace_back(vbase + 2);
    indices.emplace_back(vbase + 8);
    indices.emplace_back(vbase + 9);

    indices.emplace_back(vbase + 2);
    indices.emplace_back(vbase + 9);
    indices.emplace_back(vbase + 3);

    indices.emplace_back(vbase + 9);
    indices.emplace_back(vbase + 10);
    indices.emplace_back(vbase + 3);

    //
    //  triangle of vertex2, vertex20_aux and vertex21_aux
    //
    indices.emplace_back(vbase + 3);
    indices.emplace_back(vbase + 10);
    indices.emplace_back(vbase + 11);

    indices.emplace_back(vbase + 3);
    indices.emplace_back(vbase + 11);
    indices.emplace_back(vbase + 0);

    indices.emplace_back(vbase + 11);
    indices.emplace_back(vbase + 4);
    indices.emplace_back(vbase + 0);
  }

private:

  Vertices m_vertices;
  Indices m_indices;

  const size_t m_tessellation;
  const float m_corner_rounding_coef;
  const float m_edge_rounding_coef;
  const float m_size;
  const CubeColorsMap m_side_to_color_map;
};

