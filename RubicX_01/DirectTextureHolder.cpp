#include "stdafx.h"
#include "DirectTextureHolder.h"

//=============================================================================
DirectTextureHolder::DirectTextureHolder(ID3D11DeviceContext* a_deviceContext)
: m_deviceContext(a_deviceContext)
, m_texture(nullptr)
{}

//=============================================================================
DirectTextureHolder::~DirectTextureHolder()
{
  if (m_texture) m_texture->Release(); 
}

//=============================================================================
ID3D11ShaderResourceView* DirectTextureHolder::GetTexture() {
  if (!m_texture)
  {
    auto succeeded = load();

    if (!succeeded)
    {
      ::MessageBoxA(nullptr, "Texture not loaded", "Error", 0);
    }
  }
  return m_texture;
}

//=============================================================================
bool DirectTextureHolder::load() {
  ID3D11Device* device;
  m_deviceContext->GetDevice(&device);

  HRESULT hr = DirectX::CreateDDSTextureFromFile(
    device,
    L"TextureColors.dds",
    nullptr,
    &m_texture
  );

  return SUCCEEDED(hr);
}

//=============================================================================