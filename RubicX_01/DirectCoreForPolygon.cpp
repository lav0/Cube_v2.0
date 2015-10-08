
#include "stdafx.h"
#include "DirectCoreForPolygon.h"

//=============================================================================
DirectCoreForPolygon::DirectCoreForPolygon(HWND& a_hwnd)
  : DirectCore(a_hwnd)
//
//
//
{
}

//=============================================================================
void DirectCoreForPolygon::buildUpPolygonAndIndices()
//
//
//
{
  SimpleVertex vertices[] =
  {
    { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
    { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
    { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) }
  };

  for (UINT i=0; i<ARRAYSIZE(vertices); ++i)
    m_polygon.push_back(vertices[i]);

  WORD indices[] =
  {
      0,1,2
  };

  for (UINT i=0; i<ARRAYSIZE(indices); ++i)
    m_indices.push_back(indices[i]);
}

//=============================================================================