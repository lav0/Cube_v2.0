#pragma once

#include "IGeometry.h"
#include "GeometricPrimitiveCustom.h"

struct IFactory
{
  virtual ~IFactory() {}

  virtual std::unique_ptr<IGeometry> CreateCubeGeometryAt(
    VECTOR3_DFN                origin,
    const CubeColorsMap&       colors,
    float                      size,
    size_t                     tessellation
    ) = 0;

  virtual std::unique_ptr<IEffectWrapper> CreateEffect(
    MATRIX4X4_DFN         view,
    MATRIX4X4_DFN         projection
  ) = 0;
};
