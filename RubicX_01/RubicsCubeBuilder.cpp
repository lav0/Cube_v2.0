#include "stdafx.h"
#include "RubicsCubeBuilder.h"


//=============================================================================
RubicsCubeBuilder::RubicsCubeBuilder(
  Dimention a_dimention,
  std::unique_ptr<RubicsCubeFactory>&& unq_factory
)
: m_dimention(a_dimention)
, m_unq_factory(std::move(unq_factory))
{
}

//=============================================================================
RubicsCubeBuilder::~RubicsCubeBuilder()
{
}

//=============================================================================
std::unique_ptr<RubicsCube> RubicsCubeBuilder::build_cube() const
{
  try
  {
    auto single_cubes = buildSubcubes();

    return std::make_unique<RubicsCube>(m_dimention, single_cubes);
  }
  catch (std::exception& e)
  {
    if (m_unq_factory->DecreaseTessellation())
    {
      return build_cube();
    }

    throw e;
  }
}


//=============================================================================
float RubicsCubeBuilder::subcubeEdge() const
{
  return (
    RubicsCube::RubicsCubeSize() - RubicsCube::Clearance()
  ) / m_dimention;
}

//=============================================================================
std::vector<std::shared_ptr<RubicsSingleCube>> RubicsCubeBuilder::buildSubcubes() const
{
  std::vector<std::shared_ptr<RubicsSingleCube>> single_cubes;

  auto bigcubeSize = RubicsCube::RubicsCubeSize();

  float d_long = bigcubeSize / m_dimention;
  float d_edge = subcubeEdge();
  float d_half = bigcubeSize / 2;

  single_cubes.reserve(m_dimention * m_dimention * m_dimention);

  for (Dimention i = 0; i < m_dimention; ++i) {
    for (Dimention j = 0; j < m_dimention; ++j) {
      for (Dimention k = 0; k < m_dimention; ++k)
      {
        DirectX::XMFLOAT3 vc_centre(
          d_long / 2 + i * d_long - d_half,
          d_long / 2 + j * d_long - d_half,
          d_long / 2 + k * d_long - d_half
          );

        float size = bigcubeSize;

        const std::array<Dimention, 3> indeces = { i, j, k };

        single_cubes.push_back(m_unq_factory->CreateCube(
          vc_centre,
          d_edge,
          indeces
        ));
      }
    }
  }

  return single_cubes;
}