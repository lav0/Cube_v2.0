#include "stdafx.h"
#include "DirectCubeFactory.h"

#include "..\DirectXTK\Inc\DDSTextureLoader.h"

using namespace DirectX;

//
//
// remove later!
#include "DirectFactory.h"



//=============================================================================
DirectCubeFactory::DirectCubeFactory(
  ID3D11DeviceContext* a_deviceContext,
  Dimention a_dimention,
  size_t a_tessellation,
  DirectX::CXMMATRIX view,
  DirectX::CXMMATRIX projection
)
: m_deviceContext(a_deviceContext)
, m_texture(nullptr)
, m_dimention(a_dimention)
, m_tessellation(a_tessellation)
{
  XMStoreFloat4x4(&m_view, view);
  XMStoreFloat4x4(&m_projection, projection);

  createTexture();

  m_geometry_factory = std::make_unique<DirectFactory>(a_deviceContext, m_texture);
}

//=============================================================================
DirectCubeFactory::~DirectCubeFactory()
{
  if (m_texture) m_texture->Release();
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

  auto geometry = m_geometry_factory->CreateCubeGeometryAt(    
    a_origin,
    colors,
    a_size,
    m_tessellation
  );

  auto effect = m_geometry_factory->CreateEffect(
    XMLoadFloat4x4(&m_view),
    XMLoadFloat4x4(&m_projection)
  );

  return std::make_unique<DirectSingleCube>(
    std::move(geometry),
    std::move(effect),
    place
  );
}

//=============================================================================
bool DirectCubeFactory::createTexture()
{
  ID3D11Device* device;
  m_deviceContext->GetDevice(&device);

  HRESULT hr(S_OK);

  // Load the Texture
  hr = DirectX::CreateDDSTextureFromFile(
    device,
    L"TextureGreen.dds",
    nullptr,
    &m_texture
  );

  return SUCCEEDED(hr);
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
