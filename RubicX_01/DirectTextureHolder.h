#pragma once 


#include "..\DirectXTK\Inc\DDSTextureLoader.h"

class DirectTextureHolder
{
public:

  DirectTextureHolder(ID3D11DeviceContext* a_deviceContext)
    : m_deviceContext(a_deviceContext)
    , m_texture(nullptr) {}

  ~DirectTextureHolder() { if (m_texture) m_texture->Release(); }

  ID3D11ShaderResourceView* GetTexture() {
    if (!m_texture) {
      load();
    }
    return m_texture;
  }

private:

  bool load() {
    ID3D11Device* device;
    m_deviceContext->GetDevice(&device);

    HRESULT hr(S_OK);

    // Load the Texture
    hr = DirectX::CreateDDSTextureFromFile(
      device,
      L"TextureGreen.dds",
      nullptr,
      &m_texture
      );

    return SUCCEEDED(hr);
  }

private:

  ID3D11DeviceContext*      m_deviceContext;
  ID3D11ShaderResourceView* m_texture;

};