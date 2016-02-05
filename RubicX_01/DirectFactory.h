#pragma once

#include "RubicsCube\IFactory.h"

#include "DirectEffectWrapper.h"
#include "DirectGeometry.h"
#include "DirectViewProjectionWrapper.h"

class DirectFactory : public IFactory
{
public:

  DirectFactory(
    _In_ ID3D11DeviceContext* deviceContext,
    ID3D11ShaderResourceView* texture,
    unsigned int wnd_width,
    unsigned int wnd_height
  );

  std::unique_ptr<IGeometry> CreateCubeGeometryAt(
    DirectX::XMFLOAT3          origin,
    const CubeColorsMap&       colors,
    float                      size,
    size_t                     tessellation
  ) override;

  std::unique_ptr<IEffectWrapper> CreateEffect(
    DirectX::CXMMATRIX         view,
    DirectX::CXMMATRIX         projection
  ) override;

  std::unique_ptr<IViewProjectionWrapper> CreateViewProjection(
    const float* eye_position
  ) override;

private:

  ID3D11DeviceContext*       m_context;
  ID3D11ShaderResourceView*  m_texture;
  
  unsigned int m_wnd_width;
  unsigned int m_wnd_height;
};