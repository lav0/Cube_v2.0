#include "stdafx.h"
#include "GeometricPrimitiveCustom.h"

#include "../RubicMath/include/rcbVector3D.h"

#include "..\DirectXTK\Inc\VertexTypes.h"

#include <vector>

using namespace DirectX;


//=============================================================================
std::map<int, eCubeSide> composeFacesMap()
{
  std::map<int, eCubeSide> faces_map;

  faces_map[0] = CS_UP;
  faces_map[1] = CS_DOWN;
  faces_map[2] = CS_FRONT;
  faces_map[3] = CS_BACK;
  faces_map[4] = CS_RIGHT;
  faces_map[5] = CS_LEFT;

  return faces_map;
}

//=============================================================================
const std::map<int, eCubeSide> GeometricPrimitiveCustom::facesMap = composeFacesMap();


//=============================================================================
void fillInIndicesContainer(std::vector<uint16_t>& indices, const size_t& vbase)
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

void closeGapInCorner(
  CXMVECTOR face_normal,
  CXMVECTOR normal_start,
  CXMVECTOR normal_end,
  CXMVECTOR corner_centre,
  CXMVECTOR texture_coord,
  float corner_radius,
  size_t tessellation,
  std::vector<uint16_t>& indices,
  std::vector<VertexPositionNormalTexture>& vertices
)
{
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

void buildTorusOnCorner(  
  CXMVECTOR normal_start,
  CXMVECTOR normal_end,
  CXMVECTOR torus_centre,
  CXMVECTOR texture_coord,
  float tube_radius,
  float torus_radius,
  size_t tessellation,
  std::vector<uint16_t>& indices,
  std::vector<VertexPositionNormalTexture>& vertices
)
{
  size_t vbase = vertices.size();
  size_t stride = tessellation + 1;
  
  XMVECTOR axis = XMVector3Normalize(XMVector3Cross(normal_start, normal_end));
  XMVECTOR tangent = XMVector3Rotate(
                                     normal_start,
                                     XMQuaternionRotationAxis(axis, -XM_PIDIV2)
                                     );

  auto section_angle = XMVectorGetX(
    XMVector3AngleBetweenNormals(normal_start, normal_end));

  for (size_t i=0; i <= tessellation; ++i)
  {
    float torus_angle = i * section_angle / tessellation;
    
    XMVECTOR rotation     = XMQuaternionRotationAxis(axis, torus_angle);
    XMVECTOR torus_normal = XMVector3Normalize(XMVector3Rotate(normal_start, rotation));
    XMVECTOR torus_axis   = XMVector3Normalize(XMVector3Rotate(tangent, rotation));

    for (size_t j=0; j <= tessellation; ++j)
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

//=============================================================================
void buildQuaterCylinderOnEdge(
  CXMVECTOR normal_1, 
  CXMVECTOR normal_2, 
  CXMVECTOR bottom_centre,
  CXMVECTOR top_centre,
  const CXMVECTOR texture_coord1,
  const CXMVECTOR texture_coord2,
  float radius, 
  size_t tessellation, 
  std::vector<uint16_t>& indices,
  std::vector<VertexPositionNormalTexture>& vertices
)
{
  size_t vbase = vertices.size();
  
  XMVECTOR axis = XMVector3Normalize(XMVector3Cross(normal_1, normal_2));

  for (size_t i=0; i <= tessellation; ++i)
  {
    float angle = i * XM_PIDIV2 / tessellation;

    XMVECTOR rotation   = XMQuaternionRotationAxis(axis, angle);
    XMVECTOR rad_vector = radius * XMVector3Rotate(normal_1, rotation);

    XMVECTOR point_bottom = bottom_centre + rad_vector;
    XMVECTOR point_top    = top_centre    + rad_vector;
    
    vertices.emplace_back(point_bottom, rad_vector, texture_coord1);
    vertices.emplace_back(point_top,    rad_vector, texture_coord2);
        
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

//=============================================================================
//
//=============================================================================
std::unique_ptr<DirectX::GeometricPrimitive> GeometricPrimitiveCustom::CreateCubeAt(
  _In_ ID3D11DeviceContext* deviceContext, 
  CXMVECTOR                 centre, 
  float                     size, 
  float                     corner_rounding_coef,
  size_t                    tessellation, 
  const CubeColorsMap&      colors,
  bool                      rhcoords
)
{
  assert(corner_rounding_coef >= 0);
  assert(corner_rounding_coef < 1);

  // A cube has six faces, each one pointing in a different direction.
    const int FaceCount = 6;

    static const XMVECTORF32 faceNormals[FaceCount] =
    {
        {  0,  0,  1 },
        {  0,  0, -1 },
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
    };
    
    auto zero = 0.01f;
    static const XMVECTORF32 textureCoordinates[28] =
    {   // offset = 0 
        { 0, 0 },
        { 0, 1 },
        {  1.f / 7 - zero, 1},
        {  1.f / 7 - zero, 0 },

        // offset = 4 
        {  1.f / 7 + zero, 0 },
        {  1.f / 7 + zero, 1 },
        {  2.f / 7 - zero, 1 },
        {  2.f / 7 - zero, 0 },
        
        // offset = 8
        {  2.f / 7 + zero, 0 },
        {  2.f / 7 + zero, 1 },
        {  3.f / 7 - zero, 1 },
        {  3.f / 7 - zero, 0 },
        
        // offset = 12 
        {  3.f / 7 + zero, 0 },
        {  3.f / 7 + zero, 1 },
        {  4.f / 7 - zero, 1 },
        {  4.f / 7 - zero, 0 },
        
        // offset = 16 
        {  4.f / 7 + zero, 0 },
        {  4.f / 7 + zero, 1 },
        {  5.f / 7 - zero, 1 },
        {  5.f / 7 - zero, 0 },
        
        // offset = 20 
        {  5.f / 7 + zero, 0 },
        {  5.f / 7 + zero, 1 },
        {  6.f / 7 - zero, 1 },
        {  6.f / 7 - zero, 0 },
        
        // offset = 24 
        {  6.f / 7 + zero, 0 },
        {  6.f / 7 + zero, 1 },
        {  1.f, 1 },
        {  1.f, 0 }
    };

    std::vector<VertexPositionNormalTexture> vertices;
    std::vector<uint16_t>                    indices;

    {
      const short FACE_SIDE_VERTEX_COUNT = 12;
      const short EDGES_COUNT = 12;
      const short CORNERS_PER_FACE = 4;

      // calculate what size *vertices* and *indices* will have.
      size_t vertices_count =
        FACE_SIDE_VERTEX_COUNT * FaceCount +
        FaceCount * CORNERS_PER_FACE * (tessellation + 1) +
        EDGES_COUNT * 2 * (tessellation + 1) +
        24 * (tessellation + 1) * (tessellation + 1);

      size_t indices_count =
        FaceCount * (4 * 9 + 6) +
        3 * (FaceCount * CORNERS_PER_FACE * (tessellation - 1)) +
        EDGES_COUNT * 6 * tessellation +
        24 * 6 * tessellation * tessellation;

      vertices.reserve(vertices_count);
      indices.reserve(indices_count);
    }

    size /= 2;

    auto indent = size * corner_rounding_coef;

    auto edge_rounding_coef = corner_rounding_coef * 0.8;

    auto closeGaps =
      [&centre, &size, &corner_rounding_coef, &edge_rounding_coef, &tessellation, &indices, &vertices]
    (CXMVECTOR face_normal, CXMVECTOR normal1, CXMVECTOR texture)
    {
      assert(is_zero_dbl(XMVectorGetX(XMVector3Dot(face_normal, normal1))));

      XMVECTOR axis = XMVector3Normalize(face_normal);

      XMVECTOR start = normal1;
      auto step = XM_PIDIV2;
      for (float travel = 0.f; travel < XM_2PI - 0.01f; travel += step)
      {
        XMVECTOR end = XMVector3Rotate(start, XMQuaternionRotationAxis(axis, -step));

        XMVECTOR point = XMVectorScale(
          (start + end) * (1 - edge_rounding_coef) * (1 - corner_rounding_coef) + face_normal,
          size
          );

        closeGapInCorner(face_normal, end, start, centre + point, texture,
          size * corner_rounding_coef * (1 - edge_rounding_coef), tessellation, indices, vertices);

        start = end;
      }
    };

    // Create each face in turn.
    for (int i = 0; i < FaceCount; i++)
    {
        XMVECTOR normal = faceNormals[i];

        // Get two vectors perpendicular both to the face normal and to each other.
        XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

        XMVECTOR side1 = XMVector3Cross(normal, basis);
        XMVECTOR side2 = XMVector3Cross(normal, side1);

        XMVECTOR side1_main = XMVectorScale(
          side1, (1-corner_rounding_coef) * (1-edge_rounding_coef));

        XMVECTOR side2_main = XMVectorScale(
          side2, (1-corner_rounding_coef) * (1-edge_rounding_coef));

        XMVECTOR side1_aux = XMVectorScale(
          side1, corner_rounding_coef * (1-edge_rounding_coef));

        XMVECTOR side2_aux = XMVectorScale(
          side2, corner_rounding_coef * (1-edge_rounding_coef));

        XMVECTOR vertex0 = centre + (normal - side1_main - side2_main) * size;
        XMVECTOR vertex1 = centre + (normal - side1_main + side2_main) * size;
        XMVECTOR vertex2 = centre + (normal + side1_main + side2_main) * size;
        XMVECTOR vertex3 = centre + (normal + side1_main - side2_main) * size;

        XMVECTOR vertex00_aux = vertex0 - side2_aux * size;
        XMVECTOR vertex01_aux = vertex0 - side1_aux * size;
        XMVECTOR vertex10_aux = vertex1 - side1_aux * size;
        XMVECTOR vertex11_aux = vertex1 + side2_aux * size;
        XMVECTOR vertex20_aux = vertex2 + side2_aux * size;
        XMVECTOR vertex21_aux = vertex2 + side1_aux * size;
        XMVECTOR vertex30_aux = vertex3 + side1_aux * size;
        XMVECTOR vertex31_aux = vertex3 - side2_aux * size;
        
        fillInIndicesContainer(indices, vertices.size());

        eCubeSide thisSide = facesMap.at(i);

        int textureShift = static_cast<int>(colors.at(thisSide));

        // Four main vertices per face.
        vertices.emplace_back(vertex0, normal, textureCoordinates[textureShift + 0]);
        vertices.emplace_back(vertex1, normal, textureCoordinates[textureShift + 1]);
        vertices.emplace_back(vertex2, normal, textureCoordinates[textureShift + 2]);
        vertices.emplace_back(vertex3, normal, textureCoordinates[textureShift + 3]);
        
        // Extra eight (two in each corner) vertices per face
        vertices.emplace_back(vertex00_aux, normal, textureCoordinates[textureShift + 0]);
        vertices.emplace_back(vertex01_aux, normal, textureCoordinates[textureShift + 1]);
        vertices.emplace_back(vertex10_aux, normal, textureCoordinates[textureShift + 2]);
        vertices.emplace_back(vertex11_aux, normal, textureCoordinates[textureShift + 3]);
        vertices.emplace_back(vertex20_aux, normal, textureCoordinates[textureShift + 0]);
        vertices.emplace_back(vertex21_aux, normal, textureCoordinates[textureShift + 1]);
        vertices.emplace_back(vertex30_aux, normal, textureCoordinates[textureShift + 2]);
        vertices.emplace_back(vertex31_aux, normal, textureCoordinates[textureShift + 3]);

        // close quater-circle gaps in corners on each surface
        closeGaps(faceNormals[i], faceNormals[i > 1 ? 0 : 2], textureCoordinates[textureShift + 0]);
    }

    auto slickEdges = 
      [&centre, &size, &corner_rounding_coef, &edge_rounding_coef, &tessellation, &indices, &vertices]
      (CXMVECTOR normal1, CXMVECTOR normal2)
    {
      XMVECTOR normal3 = XMVector3Cross(normal1, normal2);

      XMVECTOR left_point  = XMVectorScale(
        normal1 + normal2 - normal3*(1-corner_rounding_coef), 
        size*(1-edge_rounding_coef)
      );
      XMVECTOR right_point = XMVectorScale(
        normal1 + normal2 + normal3*(1-corner_rounding_coef), 
        size*(1-edge_rounding_coef)
      );
      
      buildQuaterCylinderOnEdge(normal1, normal2, centre + left_point, centre + right_point, 
        textureCoordinates[0], textureCoordinates[1], 
        size * edge_rounding_coef, tessellation, indices, vertices);    
    };
      
    slickEdges(faceNormals[0], faceNormals[2]);
    slickEdges(faceNormals[0], faceNormals[3]);
    slickEdges(faceNormals[0], faceNormals[4]);
    slickEdges(faceNormals[0], faceNormals[5]);
    slickEdges(faceNormals[1], faceNormals[2]);
    slickEdges(faceNormals[1], faceNormals[3]);
    slickEdges(faceNormals[1], faceNormals[4]);
    slickEdges(faceNormals[1], faceNormals[5]);
    slickEdges(faceNormals[2], faceNormals[4]);
    slickEdges(faceNormals[2], faceNormals[5]);
    slickEdges(faceNormals[3], faceNormals[4]);
    slickEdges(faceNormals[3], faceNormals[5]);
    
    auto slickCorners = 
      [&centre, &size, &corner_rounding_coef, &edge_rounding_coef, &tessellation, &indices, &vertices]
      (CXMVECTOR normal1, CXMVECTOR normal2)
    {
      XMVECTOR normal3 = XMVector3Cross(normal1, normal2);

      XMVECTOR point  = XMVectorScale(
        (normal1 + normal2)*(1-corner_rounding_coef) + normal3, 
        size*(1-edge_rounding_coef)
      );

      buildTorusOnCorner(normal1, normal2, centre + point, textureCoordinates[0], 
        size*edge_rounding_coef, size*(1-edge_rounding_coef)*corner_rounding_coef, tessellation, indices, vertices);
    
    };

    slickCorners(faceNormals[0], faceNormals[2]);
    slickCorners(faceNormals[2], faceNormals[0]);
    slickCorners(faceNormals[0], faceNormals[4]);
    slickCorners(faceNormals[4], faceNormals[0]);
    slickCorners(faceNormals[4], faceNormals[2]);
    slickCorners(faceNormals[2], faceNormals[4]);
    
    slickCorners(faceNormals[1], faceNormals[2]);
    slickCorners(faceNormals[2], faceNormals[1]);
    slickCorners(faceNormals[1], faceNormals[4]);
    slickCorners(faceNormals[4], faceNormals[1]);
    
    slickCorners(faceNormals[0], faceNormals[3]);
    slickCorners(faceNormals[3], faceNormals[0]);
    slickCorners(faceNormals[4], faceNormals[3]);
    slickCorners(faceNormals[3], faceNormals[4]);
    
    slickCorners(faceNormals[1], faceNormals[3]);
    slickCorners(faceNormals[3], faceNormals[1]);
    
    slickCorners(faceNormals[0], faceNormals[5]);
    slickCorners(faceNormals[5], faceNormals[0]);
    slickCorners(faceNormals[5], faceNormals[2]);
    slickCorners(faceNormals[2], faceNormals[5]);
    
    slickCorners(faceNormals[1], faceNormals[5]);
    slickCorners(faceNormals[5], faceNormals[1]);
    slickCorners(faceNormals[5], faceNormals[3]);
    slickCorners(faceNormals[3], faceNormals[5]);

    return Initialize(deviceContext, vertices, indices, rhcoords);
}


//=============================================================================