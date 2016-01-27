#pragma once


#include "..\DirectXTK\Inc\GeometricPrimitive.h"

#include "../RubicMath/include/rcbVector3D.h"
#include "../RubicMath/include/rcbCubeState.h"

#include <map>
#include <vector>
#include <memory>
#include <algorithm>

enum eCubeColor
{
  CC_BLACK = 0,
  CC_GREEN = 4,
  CC_RED = 8,
  CC_BLUE = 12,
  CC_ORANGE = 16,
  CC_YELLOW = 20,
  CC_TURQUOISE = 24
};

using CubeColorsMap = std::map<eCubeSide, eCubeColor>;
using Vertices = std::vector<DirectX::VertexPositionNormalTexture>;
using Indices = std::vector<uint16_t>;

class CustomPrimitiveBuilder
{
public:

  CustomPrimitiveBuilder(
    size_t               tessellation,
    float                corner_rounding_coef,
    float                size,
    const CubeColorsMap& colors);

  ~CustomPrimitiveBuilder();
  
  bool CreateFaces(DirectX::CXMVECTOR centre);

  bool CreateEdges(DirectX::CXMVECTOR centre);

  bool CreateCorners(DirectX::CXMVECTOR centre);

  Vertices& GetVertices();
  Indices& GetIndices();

private:

  void closeGaps(DirectX::CXMVECTOR centre, 
                 DirectX::CXMVECTOR face_normal, 
                 DirectX::CXMVECTOR normal1, 
                 DirectX::CXMVECTOR texture);
  
  void slickEdges(DirectX::CXMVECTOR centre,
                  DirectX::CXMVECTOR normal1,
                  DirectX::CXMVECTOR normal2);
  
  void slickCorners(DirectX::CXMVECTOR centre,
                    DirectX::CXMVECTOR normal1,
                    DirectX::CXMVECTOR normal2);

  static void closeGapInCorner(
    DirectX::CXMVECTOR face_normal,
    DirectX::CXMVECTOR normal_start,
    DirectX::CXMVECTOR normal_end,
    DirectX::CXMVECTOR corner_centre,
    DirectX::CXMVECTOR texture_coord,
    float corner_radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<DirectX::VertexPositionNormalTexture>& vertices
  );

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
    std::vector<DirectX::VertexPositionNormalTexture>& vertices
  );

  static void buildTorusOnCorner(
    DirectX::CXMVECTOR normal_start,
    DirectX::CXMVECTOR normal_end,
    DirectX::CXMVECTOR torus_centre,
    DirectX::CXMVECTOR texture_coord,
    float tube_radius,
    float torus_radius,
    size_t tessellation,
    std::vector<uint16_t>& indices,
    std::vector<DirectX::VertexPositionNormalTexture>& vertices
  );

  static void fillInIndicesContainer(Indices& indices, const size_t& vbase);

private:

  Vertices m_vertices;
  Indices m_indices;

  const size_t m_tessellation;
  const float m_corner_rounding_coef;
  const float m_edge_rounding_coef;
  const float m_size;
  const CubeColorsMap m_side_to_color_map;

  const int m_faceCount = 6;
  static const float m_textureBorderShift;
  static DirectX::XMVECTORF32 m_faceNormals[6];
  static DirectX::XMVECTORF32 m_textureCoordinates[28];

  static const std::map<int, eCubeSide> facesMap;
};

