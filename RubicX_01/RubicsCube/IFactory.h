#pragma once

#include "IGeometry.h"
#include "IViewProjectionWrapper.h"
#include "CustomPrimitiveCubeInfo.h"

struct IFactory
{
  virtual ~IFactory() {}

  virtual std::unique_ptr<IGeometry> CreateCubeGeometryAt(
    STORE_VECTOR3_DFN    origin,
    const CubeColorsMap& colors,
    float                size,
    size_t               tessellation
    ) = 0;

  virtual std::unique_ptr<IEffectWrapper> CreateEffect(
    const MATRIX4X4_DFN& view,
    const MATRIX4X4_DFN& projection
  ) = 0;

  virtual std::unique_ptr<IViewProjectionWrapper> CreateViewProjection(
    const float* eye_position
  ) = 0;
};
