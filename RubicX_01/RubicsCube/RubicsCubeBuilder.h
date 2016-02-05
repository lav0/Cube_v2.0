#pragma once

#include "RubicsCubeFactory.h"
#include "RubicsCube.h"

class RubicsCubeBuilder
{
public:
  
  RubicsCubeBuilder(
    Dimention a_dimention,
    std::unique_ptr<RubicsCubeFactory>&& unq_factory
  );

  std::unique_ptr<RubicsCube> BuildCube() const;

private:
  
  std::vector<std::shared_ptr<RubicsSingleCube>> buildSubcubes() const;

  float subcubeEdge() const;

private:

  Dimention m_dimention;

  std::unique_ptr<RubicsCubeFactory> m_unq_factory;
};

