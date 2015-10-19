#pragma once

#include "stdafx.h"

#include "DirectSingleCube.h"

#include <memory>
#include <array>

class DirectCubeFactory
{
public:

  DirectCubeFactory(
    ID3D11DeviceContext* deviceContext,
    ID3D11ShaderResourceView* a_texture,
    Dimention a_dimention,
    size_t a_tessellation,
    DirectX::CXMMATRIX view,
    DirectX::CXMMATRIX projection
  );

  virtual ~DirectCubeFactory() {}

  virtual std::unique_ptr<DirectSingleCube> CreateCube(
    const DirectX::XMFLOAT3&        origin, 
    const float                     size,
    const std::array<Dimention,3>&  cubeIndeces
  ) const;

  bool DecreaseTessellation();

private:

  static const CubeColorsMap defaultColors();
  const CubeColorsMap customColors(const std::array<Dimention,3>& cubeIndeces) const;
  
private:

  ID3D11DeviceContext*      m_deviceContext;
  ID3D11ShaderResourceView* m_texture;

  const Dimention           m_dimention;
  size_t                    m_tessellation;

  DirectX::XMFLOAT4X4                m_view;
  DirectX::XMFLOAT4X4                m_projection;

};