#pragma once

#include "IGeometry.h"

struct IFactory
{
  virtual std::unique_ptr<IGeometry> CreateCubeGeometryAt(
    DirectX::XMFLOAT3          origin,
    const CubeColorsMap&       colors,
    float                      size,
    size_t                     tessellation
    ) = 0;

  virtual std::unique_ptr<IEffectWrapper> CreateEffect(
    DirectX::CXMMATRIX         view,
    DirectX::CXMMATRIX         projection
  ) = 0;
};
