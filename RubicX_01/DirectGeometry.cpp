
#include "DirectGeometry.h"

using namespace DirectX;

//=============================================================================
DirectGeometry::DirectGeometry(
  std::unique_ptr<GeometricPrimitive>&& geometry
)
: m_geometry(std::move(geometry))
{
}

//=============================================================================
void DirectGeometry::Draw(
  IEffectWrapper* effect
)
{
  m_geometry->Draw(
    (BasicEffect*) effect->GetEffect(), 
    (ID3D11InputLayout*)effect->GetLayout()
  );
}

//=============================================================================