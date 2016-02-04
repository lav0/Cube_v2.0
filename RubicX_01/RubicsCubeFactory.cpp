#include "stdafx.h"
#include "RubicsCubeFactory.h"

//=============================================================================
RubicsCubeFactory::RubicsCubeFactory(
  std::unique_ptr<IFactory>&& renderer_factory,
  std::unique_ptr<IViewProjectionWrapper>&& view_projection,
  Dimention a_dimention,
  size_t a_tessellation
)
: m_renderer_factory(std::move(renderer_factory))
, m_view_projection(std::move(view_projection))
, m_dimention(a_dimention)
, m_tessellation(a_tessellation)
{
}

//=============================================================================
std::unique_ptr<RubicsSingleCube> RubicsCubeFactory::CreateCube(
  const STORE_VECTOR3_DFN& a_origin,
  const float a_size,
  const std::array<Dimention,3>&  cubeIndeces
) const
{
  CubeColorsMap   colors(customColors(cubeIndeces));
  rcbCubePosition place(m_dimention-1, cubeIndeces[0], cubeIndeces[1], cubeIndeces[2]);

  auto xv = SharedFunctions::castToMatrix(m_view_projection->GetView());
  auto xp = SharedFunctions::castToMatrix(m_view_projection->GetProjection());

  auto effect = m_renderer_factory->CreateEffect(
    xv,
    xp
  );

  auto geometry = m_renderer_factory->CreateCubeGeometryAt(
    a_origin,
    colors,
    a_size,
    m_tessellation
  );

  return std::make_unique<RubicsSingleCube>(
    std::move(geometry),
    std::move(effect),
    place
  );
}

//=============================================================================
const CubeColorsMap RubicsCubeFactory::defaultColors()
{
  CubeColorsMap map;

  map[CS_UP    ] = CC_BLACK;
  map[CS_DOWN  ] = CC_BLACK;
  map[CS_FRONT ] = CC_BLACK;
  map[CS_BACK  ] = CC_BLACK;
  map[CS_RIGHT ] = CC_BLACK;
  map[CS_LEFT  ] = CC_BLACK;

  return map;
}

//=============================================================================
const CubeColorsMap RubicsCubeFactory::customColors(
  const std::array<Dimention,3>& cubeIndeces
) const
{
  
  Dimention maxIndex = m_dimention - 1;

  auto map = defaultColors();

  if (cubeIndeces[2] == 0) {
    map[CS_DOWN] = CC_RED;
  } else if (cubeIndeces[2] == maxIndex) {
    map[CS_UP] = CC_TURQUOISE;
  }

  if (cubeIndeces[1] == 0) {
    map[CS_LEFT] = CC_GREEN;
  } else if (cubeIndeces[1] == maxIndex) {
    map[CS_RIGHT] = CC_BLUE;
  }
    
  if (cubeIndeces[0] == 0) {
    map[CS_BACK] = CC_ORANGE;
  } else if (cubeIndeces[0] == maxIndex) {
    map[CS_FRONT] = CC_YELLOW;
  }
  
  return map;
}

//=============================================================================
bool RubicsCubeFactory::DecreaseTessellation()
{  
  auto t = m_tessellation;

  m_tessellation /= 2;

  return m_tessellation != t;
}

//=============================================================================
