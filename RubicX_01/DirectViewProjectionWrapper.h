#pragma once

#include "IGeometry.h"
#include <DirectXMath.h>

static const float fovAngle = DirectX::XM_PIDIV2;

class DirectViewProjectionWrapper : public IViewProjectionWrapper
{
public:

  DirectViewProjectionWrapper();

  bool Initialize(unsigned int width,
    unsigned int height,
    const float* eye_position) override;

  void* GetView() const;
  void* GetProjection() const;

private:

  DirectX::XMFLOAT4X4 m_view;
  DirectX::XMFLOAT4X4 m_proj;

};