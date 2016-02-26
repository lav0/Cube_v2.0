
#include "DirectTextureHolder.h"

//=============================================================================
DirectTextureHolder::DirectTextureHolder(ID3D11DeviceContext* a_deviceContext)
: m_deviceContext(a_deviceContext)
{}

//=============================================================================
DirectTextureHolder::~DirectTextureHolder()
{
  for (auto item : m_texture_collection)
  {
    if (item.second)
      item.second->Release();
  }
}

//=============================================================================
ID3D11ShaderResourceView* DirectTextureHolder::GetTexture(const wchar_t* texture_name)
{
  auto itr = m_texture_collection.find(texture_name);

  if (itr == m_texture_collection.end())
  {
    auto succeeded = load(texture_name);

    if (!succeeded)
    {
      ::MessageBoxA(nullptr, "Texture not loaded", "Error", 0);

      return nullptr;
    }
  }

  return m_texture_collection[texture_name];
}

//=============================================================================
bool DirectTextureHolder::load(const wchar_t* texture_name)
{
  ID3D11Device* device;
  m_deviceContext->GetDevice(&device);

  ID3D11ShaderResourceView* texture(nullptr);

  HRESULT hr = DirectX::CreateDDSTextureFromFile(
    device,
    texture_name,
    nullptr,
    &texture
    );

  m_texture_collection.emplace(texture_name, texture);

  return SUCCEEDED(hr);
}

//=============================================================================