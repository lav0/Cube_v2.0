#include "stdafx.h"
#include "DirectCubeFactory.h"


using namespace DirectX;

//=============================================================================
DirectCubeFactory::DirectCubeFactory(
  ID3D11DeviceContext* a_deviceContext,
  ID3D11ShaderResourceView* a_texture,
  Dimention a_dimention
)
: m_deviceContext(a_deviceContext)
, m_texture(a_texture)
, m_dimention(a_dimention)
, m_tessellation(8)
{
}


//=============================================================================
std::unique_ptr<DirectSingleCube> DirectCubeFactory::CreateCube(
  const DirectX::XMFLOAT3& a_origin, 
  const float a_size,
  const std::array<Dimention,3>&  cubeIndeces
) const
{
  CubeColorsMap   colors(customColors(cubeIndeces));
  rcbCubePosition place(m_dimention-1, cubeIndeces[0], cubeIndeces[1], cubeIndeces[2]);

  return std::make_unique<DirectSingleCube>(
    m_deviceContext, 
    m_texture, 
    a_origin, 
    colors, 
    place,
    a_size,
    m_tessellation
  );
}


//=============================================================================
const CubeColorsMap DirectCubeFactory::defaultColors()
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
const CubeColorsMap DirectCubeFactory::customColors(
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
bool DirectCubeFactory::DecreaseTessellation()
{  
  auto t = m_tessellation;

  m_tessellation /= 2;

  return m_tessellation != t;
}

//=============================================================================