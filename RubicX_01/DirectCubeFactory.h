#pragma once

#include "stdafx.h"

#include "IFactory.h"
#include "DirectSingleCube.h"

#include <memory>
#include <array>

class DirectCubeFactory
{
public:

  DirectCubeFactory(
    std::unique_ptr<IFactory>&& geometry_factory,
    Dimention a_dimention,
    size_t a_tessellation,
    DirectX::CXMMATRIX view,
    DirectX::CXMMATRIX projection
  );

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

  std::unique_ptr<IFactory> m_geometry_factory;

  const Dimention           m_dimention;
  size_t                    m_tessellation;

  DirectX::XMFLOAT4X4       m_view;
  DirectX::XMFLOAT4X4       m_projection;

};