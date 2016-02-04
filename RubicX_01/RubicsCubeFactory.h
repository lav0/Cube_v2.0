#pragma once

#include "stdafx.h"

#include "IFactory.h"
#include "RubicsSingleCube.h"
#include "SharedFunctions.h"

#include <memory>
#include <array>

class RubicsCubeFactory
{
public:

  RubicsCubeFactory(
    std::unique_ptr<IFactory>&& renderer_factory,
    std::unique_ptr<IViewProjectionWrapper>&& view_projection,
    Dimention a_dimention,
    size_t a_tessellation
  );

  virtual std::unique_ptr<RubicsSingleCube> CreateCube(
    const STORE_VECTOR3_DFN&        origin,
    const float                     size,
    const std::array<Dimention,3>&  cubeIndeces
  ) const;

  bool DecreaseTessellation();

private:

  static const CubeColorsMap defaultColors();
  const CubeColorsMap customColors(const std::array<Dimention,3>& cubeIndeces) const;
    
private:

  std::unique_ptr<IFactory>               m_renderer_factory;
  std::unique_ptr<IViewProjectionWrapper> m_view_projection;

  const Dimention           m_dimention;
  size_t                    m_tessellation;
};