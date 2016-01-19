#pragma once

#include "DirectCubeFactory.h"
#include "DirectRubicsCube.h"

class RubicsCubeBuilder
{
public:
  
  RubicsCubeBuilder(
    Dimention a_dimention,
    std::unique_ptr<DirectCubeFactory>&& unq_factory
  );

  std::unique_ptr<DirectRubicsCube> build_cube() const;

  ~RubicsCubeBuilder();

private:
  
  std::vector<std::shared_ptr<DirectSingleCube>> buildSubcubes() const;

  float subcubeEdge() const;

private:

  Dimention m_dimention;

  std::unique_ptr<DirectCubeFactory> m_unq_factory;
};

