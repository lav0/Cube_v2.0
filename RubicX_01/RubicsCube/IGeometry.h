#pragma once

#include "IEffectWrapper.h"

struct IGeometry
{
  virtual ~IGeometry() {}

  virtual void Draw(IEffectWrapper* effect_wrapper) = 0;
};

