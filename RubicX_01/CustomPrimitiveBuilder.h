#pragma once


#include "CustomPrimitiveCubeInfo.h"

#include "../RubicMath/include/rcbVector3D.h"

#include <vector>
#include <memory>
#include <algorithm>

template <typename VertexType>
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
    , m_edge_rounding_coef(0.4 * corner_rounding_coef)
    , m_size(0.5 * size)
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

  bool CreateFaces(DirectX::CXMVECTOR centre)
  {
    using namespace DirectX;
    using namespace CustomPrimitiveCubeInfo;

    // Create each face in turn.
    for (int i = 0; i < m_faceCount; i++)
    {
      XMVECTOR normal = m_faceNormals[i];

      // Get two vectors perpendicular both to the face normal and to each other.
      XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

      XMVECTOR side1 = XMVector3Cross(normal, basis);
      XMVECTOR side2 = XMVector3Cross(normal, side1);

      XMVECTOR side1_main = XMVectorScale(
        side1, (1 - m_corner_rounding_coef) * (1 - m_edge_rounding_coef));

      XMVECTOR side2_main = XMVectorScale(
        side2, (1 - m_corner_rounding_coef) * (1 - m_edge_rounding_coef));

      XMVECTOR side1_aux = XMVectorScale(
        side1, m_corner_rounding_coef * (1 - m_edge_rounding_coef));

      XMVECTOR side2_aux = XMVectorScale(
        side2, m_corner_rounding_coef * (1 - m_edge_rounding_coef));

      XMVECTOR vertex0 = centre + (normal - side1_main - side2_main) * m_size;
      XMVECTOR vertex1 = centre + (normal - side1_main + side2_main) * m_size;
      XMVECTOR vertex2 = centre + (normal + side1_main + side2_main) * m_size;
      XMVECTOR vertex3 = centre + (normal + side1_main - side2_main) * m_size;

      XMVECTOR vertex00_aux = vertex0 - side2_aux * m_size;
      XMVECTOR vertex01_aux = vertex0 - side1_aux * m_size;
      XMVECTOR vertex10_aux = vertex1 - side1_aux * m_size;
      XMVECTOR vertex11_aux = vertex1 + side2_aux * m_size;
      XMVECTOR vertex20_aux = vertex2 + side2_aux * m_size;
      XMVECTOR vertex21_aux = vertex2 + side1_aux * m_size;
      XMVECTOR vertex30_aux = vertex3 + side1_aux * m_size;
      XMVECTOR vertex31_aux = vertex3 - side2_aux * m_size;

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

  bool CreateEdges(DirectX::CXMVECTOR centre)
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

  bool CreateCorners(DirectX::CXMVECTOR centre)
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

  void closeGaps(DirectX::CXMVECTOR centre, 
                 DirectX::CXMVECTOR face_normal, 
                 DirectX::CXMVECTOR normal1, 
                 DirectX::CXMVECTOR texture)
  {
    using namespace DirectX;

    assert(is_zero_dbl(XMVectorGetX(XMVector3Dot(face_normal, normal1))));

    XMVECTOR axis = XMVector3Normalize(face_normal);

    XMVECTOR start = normal1;
    auto step = XM_PIDIV2;
    for (float travel = 0.f; travel < XM_2PI - 0.01f; travel += step)
    {
      XMVECTOR end = XMVector3Rotate(start, XMQuaternionRotationAxis(axis, -step));

      XMVECTOR point = XMVectorScale(
        (start + end) * (1 - m_edge_rounding_coef) * (1 - m_corner_rounding_coef) + face_normal,
        m_size
        );

      closeGapInCorner(face_normal, end, start, centre + point, texture,
        m_size * m_corner_rounding_coef * (1 - m_edge_rounding_coef),
        m_tessellation,
        m_indices, m_vertices);

      start = end;
    }
  }
  
  void slickEdges(DirectX::CXMVECTOR centre,
                  DirectX::CXMVECTOR normal1,
                  DirectX::CXMVECTOR normal2)
  {
    using namespace DirectX;
    using namespace CustomPrimitiveCubeInfo;

    XMVECTOR normal3 = XMVector3Cross(normal1, normal2);

    XMVECTOR left_point = XMVectorScale(
      normal1 + normal2 - normal3*(1 - m_corner_rounding_coef),
      m_size*(1 - m_edge_rounding_coef)
      );
    XMVECTOR right_point = XMVectorScale(
      normal1 + normal2 + normal3*(1 - m_corner_rounding_coef),
      m_size*(1 - m_edge_rounding_coef)
      );

    buildQuaterCylinderOnEdge(normal1, normal2, centre + left_point, centre + right_point,
      m_textureCoordinates[0], m_textureCoordinates[1],
      m_size * m_edge_rounding_coef, m_tessellation, m_indices, m_vertices);
  }
  
  void slickCorners(DirectX::CXMVECTOR centre,
                    DirectX::CXMVECTOR normal1,
                    DirectX::CXMVECTOR normal2)
  {
    using namespace DirectX;
    using namespace CustomPrimitiveCubeInfo;

    XMVECTOR normal3 = XMVector3Cross(normal1, normal2);

    XMVECTOR point = XMVectorScale(
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
    DirectX::CXMVECTOR face_normal,
    DirectX::CXMVECTOR normal_start,
    DirectX::CXMVECTOR normal_end,
    DirectX::CXMVECTOR corner_centre,
    DirectX::CXMVECTOR texture_coord,
    float corner_radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<VertexType>& vertices
    )
  {
    using namespace DirectX;

    XMVECTOR axis = XMVector3Normalize(face_normal);

    size_t vbase = vertices.size();

    auto full_angle = XMVectorGetX(
      XMVector3AngleBetweenNormals(normal_start, normal_end));

    for (size_t i = 0; i <= tessellation; ++i)
    {
      auto angle = i * full_angle / tessellation;

      XMVECTOR rotation = XMQuaternionRotationAxis(axis, angle);
      XMVECTOR normal = XMVector3Normalize(XMVector3Rotate(normal_start, rotation));

      XMVECTOR point = corner_centre + corner_radius * normal;

      vertices.emplace_back(point, axis, texture_coord);

      if (i != 0 && i != 1) {
        indices.emplace_back(vbase);
        indices.emplace_back(vbase + i);
        indices.emplace_back(vbase + i - 1);
      }
    }
  }

  static void buildQuaterCylinderOnEdge(
    DirectX::CXMVECTOR normal_1,
    DirectX::CXMVECTOR normal_2,
    DirectX::CXMVECTOR bottom_centre,
    DirectX::CXMVECTOR top_centre,
    const DirectX::CXMVECTOR texture_coord1,
    const DirectX::CXMVECTOR texture_coord2,
    float radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<VertexType>& vertices
    )
  {
    using namespace DirectX;

    size_t vbase = vertices.size();

    XMVECTOR axis = XMVector3Normalize(XMVector3Cross(normal_1, normal_2));

    for (size_t i = 0; i <= tessellation; ++i)
    {
      float angle = i * XM_PIDIV2 / tessellation;

      XMVECTOR rotation = XMQuaternionRotationAxis(axis, angle);
      XMVECTOR rad_vector = radius * XMVector3Rotate(normal_1, rotation);

      XMVECTOR point_bottom = bottom_centre + rad_vector;
      XMVECTOR point_top = top_centre + rad_vector;

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
    DirectX::CXMVECTOR normal_start,
    DirectX::CXMVECTOR normal_end,
    DirectX::CXMVECTOR torus_centre,
    DirectX::CXMVECTOR texture_coord,
    float tube_radius,
    float torus_radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<VertexType>& vertices
    )
  {
    using namespace DirectX;

    size_t vbase = vertices.size();
    size_t stride = tessellation + 1;

    XMVECTOR axis = XMVector3Normalize(XMVector3Cross(normal_start, normal_end));
    XMVECTOR tangent = XMVector3Rotate(
      normal_start,
      XMQuaternionRotationAxis(axis, -XM_PIDIV2)
      );

    auto section_angle = XMVectorGetX(
      XMVector3AngleBetweenNormals(normal_start, normal_end));

    for (size_t i = 0; i <= tessellation; ++i)
    {
      float torus_angle = i * section_angle / tessellation;

      XMVECTOR rotation = XMQuaternionRotationAxis(axis, torus_angle);
      XMVECTOR torus_normal = XMVector3Normalize(XMVector3Rotate(normal_start, rotation));
      XMVECTOR torus_axis = XMVector3Normalize(XMVector3Rotate(tangent, rotation));

      for (size_t j = 0; j <= tessellation; ++j)
      {
        float tube_angle = j * XM_PIDIV2 / tessellation;

        XMVECTOR tube_rotation = XMQuaternionRotationAxis(torus_axis, tube_angle);
        XMVECTOR tube_normal = XMVector3Rotate(torus_normal, tube_rotation);

        XMVECTOR point = torus_centre + torus_radius * torus_normal + tube_radius * tube_normal;

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

