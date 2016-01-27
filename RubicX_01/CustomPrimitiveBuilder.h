#pragma once


#include "..\DirectXTK\Inc\GeometricPrimitive.h"

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

typedef std::map<eCubeSide, eCubeColor> CubeColorsMap;

using Vertices = std::vector<DirectX::VertexPositionNormalTexture>;
using Indices = std::vector<uint16_t>;

class CustomPrimitiveBuilder
{
public:

  CustomPrimitiveBuilder(size_t tessellation);
  ~CustomPrimitiveBuilder();
  
  bool CreateFaces(DirectX::CXMVECTOR            centre,
                   float                size,
                   float                corner_rounding_coef,
                   const CubeColorsMap& colors);


private:

  std::unique_ptr<Vertices> m_unq_vertices;
  std::unique_ptr<Indices> m_unq_indices;

  const size_t m_tessellation;

  const int m_faceCount = 6;
  static const float m_textureBorderShift;
  static DirectX::XMVECTORF32 m_faceNormals[6];
  static DirectX::XMVECTORF32 m_textureCoordinates[28];
};

