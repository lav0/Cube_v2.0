#pragma once 


#include "..\DirectXTK\Inc\DDSTextureLoader.h"

#include <unordered_map>

class DirectTextureHolder
{
public:

  DirectTextureHolder(ID3D11DeviceContext* a_deviceContext);

  ~DirectTextureHolder();

  ID3D11ShaderResourceView* GetTexture(const wchar_t* texture_name);

private:

  bool load(const wchar_t* texture_name);

private:

  ID3D11DeviceContext*         m_deviceContext;
  std::unordered_map<
    const wchar_t*,
    ID3D11ShaderResourceView*> m_texture_collection;
};