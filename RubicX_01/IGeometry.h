#pragma once

#ifdef RENDER_DRX
#include "GeometricPrimitiveCustom.h"
#define MATRIX4X4_DFN DirectX::CXMMATRIX
#define VECTOR3_DFN   DirectX::XMFLOAT3
#endif

struct IEffectWrapper
{
  virtual ~IEffectWrapper() {}

  virtual void* GetEffect() const = 0;
  virtual void* GetLayout() const = 0;

  virtual void SetWorld(MATRIX4X4_DFN world) = 0;
};

struct IGeometry
{
  virtual ~IGeometry() {}

  virtual void Draw(IEffectWrapper* effect_wrapper) = 0;
};

