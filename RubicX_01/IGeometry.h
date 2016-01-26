#pragma once

#include "GeometricPrimitiveCustom.h"

struct IEffectWrapper
{
  virtual void* GetEffect() const = 0;
  virtual void* GetLayout() const = 0;
};

struct IGeometry
{
  virtual ~IGeometry() {}

  virtual void Draw(IEffectWrapper* effect_wrapper) = 0;
};

