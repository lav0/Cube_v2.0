#include "stdafx.h"
#include "DirectFactory.h"

//=============================================================================
DirectFactory::DirectFactory(
  _In_ ID3D11DeviceContext* deviceContext,
  ID3D11ShaderResourceView* texture
)
: m_context(deviceContext)
, m_texture(texture)
{
}

//=============================================================================
std::unique_ptr<IGeometry> DirectFactory::CreateCubeGeometryAt(
  DirectX::XMFLOAT3          origin,
  const CubeColorsMap&       colors,
  float                      size,
  size_t                     tessellation
  )
{
  auto drx_primitive = DirectCustomPrimitiveGeometry::CreateCubeAt(
    m_context,
    XMLoadFloat3(&origin),
    size,
    0.2f,
    tessellation,
    colors,
    false
  );

  return std::make_unique<DirectGeometry>(std::move(drx_primitive));
}

//=============================================================================
std::unique_ptr<IEffectWrapper> DirectFactory::CreateEffect(
  DirectX::CXMMATRIX view,
  DirectX::CXMMATRIX projection
)
{
  return std::make_unique<DirectEffectWrapper>(
    m_context,
    m_texture,
    view,
    projection
  );
}

//=============================================================================