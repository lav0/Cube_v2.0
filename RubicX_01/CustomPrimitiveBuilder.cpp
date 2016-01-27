#include "stdafx.h"
#include "CustomPrimitiveBuilder.h"

const float CustomPrimitiveBuilder::m_textureBorderShift = 0.01f;
DirectX::XMVECTORF32 CustomPrimitiveBuilder::m_faceNormals[6] = 
{
  { 0, 0, 1 },
  { 0, 0, -1 },
  { 1, 0, 0 },
  { -1, 0, 0 },
  { 0, 1, 0 },
  { 0, -1, 0 },
};
DirectX::XMVECTORF32 CustomPrimitiveBuilder::m_textureCoordinates[28] = 
{
  { 0, 0 },
  { 0, 1 },
  { 1.f / 7 - m_textureBorderShift, 1 },
  { 1.f / 7 - m_textureBorderShift, 0 },

  // offset = 4 
  { 1.f / 7 + m_textureBorderShift, 0 },
  { 1.f / 7 + m_textureBorderShift, 1 },
  { 2.f / 7 - m_textureBorderShift, 1 },
  { 2.f / 7 - m_textureBorderShift, 0 },

  // offset = 8
  { 2.f / 7 + m_textureBorderShift, 0 },
  { 2.f / 7 + m_textureBorderShift, 1 },
  { 3.f / 7 - m_textureBorderShift, 1 },
  { 3.f / 7 - m_textureBorderShift, 0 },

  // offset = 12 
  { 3.f / 7 + m_textureBorderShift, 0 },
  { 3.f / 7 + m_textureBorderShift, 1 },
  { 4.f / 7 - m_textureBorderShift, 1 },
  { 4.f / 7 - m_textureBorderShift, 0 },

  // offset = 16 
  { 4.f / 7 + m_textureBorderShift, 0 },
  { 4.f / 7 + m_textureBorderShift, 1 },
  { 5.f / 7 - m_textureBorderShift, 1 },
  { 5.f / 7 - m_textureBorderShift, 0 },

  // offset = 20 
  { 5.f / 7 + m_textureBorderShift, 0 },
  { 5.f / 7 + m_textureBorderShift, 1 },
  { 6.f / 7 - m_textureBorderShift, 1 },
  { 6.f / 7 - m_textureBorderShift, 0 },

  // offset = 24 
  { 6.f / 7 + m_textureBorderShift, 0 },
  { 6.f / 7 + m_textureBorderShift, 1 },
  { 1.f, 1 },
  { 1.f, 0 }
};

//=============================================================================
CustomPrimitiveBuilder::CustomPrimitiveBuilder(size_t tessellation)
: m_tessellation(tessellation)
{ 
  const short FACE_SIDE_VERTEX_COUNT = 12;
  const short EDGES_COUNT = 12;
  const short CORNERS_PER_FACE = 4;

  // calculate what size *vertices* and *indices* will have.
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

  m_unq_vertices = std::make_unique<Vertices>(vertices_count);    
  m_unq_indices = std::make_unique<Indices>(indices_count);
}

//=============================================================================
CustomPrimitiveBuilder::~CustomPrimitiveBuilder()
{
}

//=============================================================================
bool CustomPrimitiveBuilder::CreateFaces(
  DirectX::CXMVECTOR   centre,
  float                size,
  float                corner_rounding_coef,
  const CubeColorsMap& colors)
{
  // NEXT STEP TO REFACTOR HERE
  return true;
}
