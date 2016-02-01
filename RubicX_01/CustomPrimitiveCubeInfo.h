#pragma once

#include "SHARED_DEFS.h"

#include "../RubicMath/include/rcbCubeState.h"

#include <map>

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

namespace CustomPrimitiveCubeInfo
{
  std::map<int, eCubeSide> composeFacesMap();

  static const int m_faceCount = 6;
  static const float m_textureBorderShift = 0.01f;
  static ALIGNED_VECTOR_DFN m_faceNormals[6] = {
    { 0, 0, 1 },
    { 0, 0, -1 },
    { 1, 0, 0 },
    { -1, 0, 0 },
    { 0, 1, 0 },
    { 0, -1, 0 },
  };;
  static ALIGNED_VECTOR_DFN m_textureCoordinates[28] =
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

  static const std::map<int, eCubeSide> facesMap = composeFacesMap();
};

