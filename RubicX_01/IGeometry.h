#pragma once

#include "SHARED_DEFS.h"
#include "../RubicMath/include/rcbQuaternion.h"

struct IEffectWrapper
{
  virtual ~IEffectWrapper() {}

  virtual void* GetEffect() const = 0;
  virtual void* GetLayout() const = 0;

  virtual void SetWorld(const MATRIX4X4_DFN& world) = 0;
  virtual void SetRotation(const rcbQuaternion& quaternion) = 0;
};

struct IViewProjectionWrapper
{
  virtual ~IViewProjectionWrapper() {}

  virtual bool Initialize(unsigned int width,
                          unsigned int height,
                          const float* eye_position) = 0;

  virtual void* GetView() const = 0;
  virtual void* GetProjection() const = 0;
};

struct IGeometry
{
  virtual ~IGeometry() {}

  virtual void Draw(IEffectWrapper* effect_wrapper) = 0;
};

