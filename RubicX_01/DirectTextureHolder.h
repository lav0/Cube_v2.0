#pragma once 


#include "..\DirectXTK\Inc\DDSTextureLoader.h"

class DirectTextureHolder
{
public:

  DirectTextureHolder(ID3D11DeviceContext* a_deviceContext);

  ~DirectTextureHolder();

  ID3D11ShaderResourceView* GetTexture();

private:

  bool load();

private:

  ID3D11DeviceContext*      m_deviceContext;
  ID3D11ShaderResourceView* m_texture;
};