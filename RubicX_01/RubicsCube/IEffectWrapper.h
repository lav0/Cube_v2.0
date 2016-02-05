#pragma once

#include "SHARED_DEFS.h"
#include "../../RubicMath/include/rcbQuaternion.h"

struct IEffectWrapper
{
  virtual ~IEffectWrapper() {}

  virtual void* GetEffect() const = 0;
  virtual void* GetLayout() const = 0;

  virtual void SetWorld(const MATRIX4X4_DFN& world) = 0;
  virtual void SetRotation(const rcbQuaternion& quaternion) = 0;
};
