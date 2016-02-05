#pragma once

#include "RubicsCube\IGeometry.h"

#include "DirectCustomPrimitiveGeometry.h"

class DirectGeometry : public IGeometry
{
public:

  DirectGeometry(
    std::unique_ptr<DirectX::GeometricPrimitive>&& geometry
  );

  void Draw(IEffectWrapper* effect_wrapper) override;

private:

  std::unique_ptr<DirectX::GeometricPrimitive> m_geometry;

};
