#include "stdafx.h"
#include "DirectEffectWrapper.h"
#include "../DirectXTK/Src/PlatformHelpers.h"
#include <wrl.h>

using namespace DirectX;

//=============================================================================
static void CreateInputLayout(
  _In_ ID3D11Device* device, 
  IEffect* effect, 
  _Outptr_ ID3D11InputLayout** pInputLayout
)
{
  assert(pInputLayout != 0);

  void const* shaderByteCode;
  size_t byteCodeLength;

  effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

  ThrowIfFailed(
    device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
    VertexPositionNormalTexture::InputElementCount,
    shaderByteCode, byteCodeLength,
    pInputLayout)
    );

  //SetDebugObjectName(*pInputLayout, "DirectXTK:GeometricPrimitive");
}


//=============================================================================
DirectEffectWrapper::DirectEffectWrapper(
  ID3D11DeviceContext*       immediate_context,
  ID3D11ShaderResourceView*  texture,
  DirectX::CXMMATRIX         view,
  DirectX::CXMMATRIX         projection
)
{
  Microsoft::WRL::ComPtr<ID3D11Device> device;
  immediate_context->GetDevice(&device);

  m_effect = std::make_unique<BasicEffect>(device.Get());
  m_effect->EnableDefaultLighting();
  m_effect->SetTextureEnabled(true);

  m_effect->SetTexture(texture);
  m_effect->SetView(view);
  m_effect->SetProjection(projection);

  setUpLighting();
  
  ::CreateInputLayout(device.Get(), m_effect.get(), &m_input_layout);
}

//=============================================================================
void* DirectEffectWrapper::GetEffect() const
{
  return m_effect.get();
}

//=============================================================================
void* DirectEffectWrapper::GetLayout() const
{
  return m_input_layout.Get();
}

//=============================================================================
void DirectEffectWrapper::setUpLighting()
{
  m_effect->SetLightEnabled(0, true);
  m_effect->SetLightEnabled(1, true);
  m_effect->SetLightEnabled(2, true);

  float lean_value = LIGHT_SOURCE_LEAN_FACTOR();

  float x0(.0), y0(.0), x1(.0), y1(.0), x2(.0), y2(.0);
  XMScalarSinCos(&x0, &y0, 0.0);
  XMScalarSinCos(&x1, &y1, 2 * XM_PI / 3);
  XMScalarSinCos(&x2, &y2, 4 * XM_PI / 3);

  float scale_factor = LIGHT_SOURCE_SCALE_FACTOR();

  XMVECTOR scale = XMVectorSet(
    scale_factor * lean_value,
    scale_factor * lean_value,
    scale_factor,
    .0f);

  XMVECTOR light0 = XMVectorMultiply(scale, XMVectorSet(x0, y0, 0.2f, .0f));
  XMVECTOR light1 = XMVectorMultiply(scale, XMVectorSet(x1, y1, 0.5f, .0f));
  XMVECTOR light2 = XMVectorMultiply(scale, XMVectorSet(x2, y2, 0.7f, .0f));

  m_effect->SetLightDirection(0, light0);
  m_effect->SetLightDirection(1, light1);
  m_effect->SetLightDirection(2, light2);
}