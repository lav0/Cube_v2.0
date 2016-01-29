#pragma once

#include "SHARED_DEFS.h"

struct IEffectWrapper
{
  virtual ~IEffectWrapper() {}

  virtual void* GetEffect() const = 0;
  virtual void* GetLayout() const = 0;

  virtual void SetWorld(const MATRIX4X4_DFN& world) = 0;
};

struct IGeometry
{
  virtual ~IGeometry() {}

  virtual void Draw(IEffectWrapper* effect_wrapper) = 0;
};

