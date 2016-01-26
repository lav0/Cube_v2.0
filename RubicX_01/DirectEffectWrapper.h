#pragma once

#include "IGeometry.h"
#include "DirectGeometry.h"
#include <wrl.h>

static const float LIGHT_SOURCE_LEAN_FACTOR() { return 0.6f; }
static const float LIGHT_SOURCE_SCALE_FACTOR() { return 0.6f; }

class DirectEffectWrapper : public IEffectWrapper
{
public:

  DirectEffectWrapper(
    ID3D11DeviceContext*       immediate_context,
    ID3D11ShaderResourceView*  texture,
    DirectX::CXMMATRIX         view,
    DirectX::CXMMATRIX         projection
  );

  void* GetEffect() const override;
  void* GetLayout() const override;
  
private:

  void setUpLighting();

private:

  std::unique_ptr<DirectX::BasicEffect> m_effect;
  
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout;

};