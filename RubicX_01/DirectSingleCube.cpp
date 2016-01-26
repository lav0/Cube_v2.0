#include "stdafx.h"
#include "DirectSingleCube.h"
#include "../DirectXTK/Src/PlatformHelpers.h"

using namespace DirectX;

//=============================================================================
DirectSingleCube::DirectSingleCube(
  std::unique_ptr<IGeometry>&& geometry,
  std::unique_ptr<IEffectWrapper>&& effect_wrapper,
  const rcbCubePosition&     place
)
  : m_geometry(std::move(geometry))
  , m_effect_wrapper(std::move(effect_wrapper))
  , m_place(place)
{
  XMStoreFloat3x3(&m_rotation, XMMatrixIdentity());
}

//=============================================================================
void DirectSingleCube::Rotate(DirectX::CXMVECTOR a_quaternion)
{
  XMMATRIX rot = XMMatrixRotationQuaternion(a_quaternion);
  XMMATRIX cur = XMLoadFloat3x3(&m_rotation) * rot;
  
  XMStoreFloat3x3(&m_rotation, cur);
}

//=============================================================================
void DirectSingleCube::Turn(eTurnAxis axis, eAngle angle)
{
  m_place.turn(axis, angle);
}

//=============================================================================
void DirectSingleCube::Draw()
{
  XMMATRIX pos = XMLoadFloat3x3(&m_rotation);

  auto* effect = (DirectX::BasicEffect*) m_effect_wrapper->GetEffect();
  effect->SetWorld(pos);

  auto* layout = (ID3D11InputLayout*)m_effect_wrapper->GetLayout();
  
  m_geometry->Draw(m_effect_wrapper.get());
}

//=============================================================================
void DirectSingleCube::Align(DirectX::CXMMATRIX basis)
//
// Align the single cube to the given position. 
// Used for making the Rubic's cube to be flat on each face
//   after rotation one of the faces.
// Container std::map<> used to find the closest basis's ort to the
//   current position (as std::map<> sorts element by its key
//   and the key here is result of scalar product).
//
{
  XMMATRIX pos = XMLoadFloat3x3(&m_rotation);
  
  auto scl00 = XMVectorGetX( XMVector3Dot(pos.r[0], basis.r[0]) );
  auto scl01 = XMVectorGetX( XMVector3Dot(pos.r[0], basis.r[1]) );
  auto scl02 = XMVectorGetX( XMVector3Dot(pos.r[0], basis.r[2]) );
  auto scl00_n = XMVectorGetX( XMVector3Dot(pos.r[0], - basis.r[0]) );
  auto scl01_n = XMVectorGetX( XMVector3Dot(pos.r[0], - basis.r[1]) );
  auto scl02_n = XMVectorGetX( XMVector3Dot(pos.r[0], - basis.r[2]) );

  std::map<float, XMFLOAT3> x_basisMap;
  XMStoreFloat3( &x_basisMap[scl00], basis.r[0] );
  XMStoreFloat3( &x_basisMap[scl01], basis.r[1] );
  XMStoreFloat3( &x_basisMap[scl02], basis.r[2] );
  XMStoreFloat3( &x_basisMap[scl00_n], - basis.r[0]);
  XMStoreFloat3( &x_basisMap[scl01_n], - basis.r[1]);
  XMStoreFloat3( &x_basisMap[scl02_n], - basis.r[2]);

  auto scl10 = XMVectorGetX( XMVector3Dot(pos.r[1], basis.r[0]) );
  auto scl11 = XMVectorGetX( XMVector3Dot(pos.r[1], basis.r[1]) );
  auto scl12 = XMVectorGetX( XMVector3Dot(pos.r[1], basis.r[2]) );
  auto scl10_n = XMVectorGetX( XMVector3Dot(pos.r[1], - basis.r[0]) );
  auto scl11_n = XMVectorGetX( XMVector3Dot(pos.r[1], - basis.r[1]) );
  auto scl12_n = XMVectorGetX( XMVector3Dot(pos.r[1], - basis.r[2]) );
    
  std::map<float, XMFLOAT3> y_basisMap;
  XMStoreFloat3( &y_basisMap[scl10], basis.r[0]);
  XMStoreFloat3( &y_basisMap[scl11], basis.r[1]);
  XMStoreFloat3( &y_basisMap[scl12], basis.r[2]);
  XMStoreFloat3( &y_basisMap[scl10_n], - basis.r[0]);
  XMStoreFloat3( &y_basisMap[scl11_n], - basis.r[1]);
  XMStoreFloat3( &y_basisMap[scl12_n], - basis.r[2]);
  
  XMVECTOR x_basis = XMLoadFloat3( &(--x_basisMap.end())->second );
  
  XMVECTOR y_basis = XMLoadFloat3( &(--y_basisMap.end())->second );

  XMVECTOR z_basis = XMVector3Cross(x_basis, y_basis);

  XMMATRIX final_basis = basis;
  final_basis.r[0] = x_basis;
  final_basis.r[1] = y_basis;
  final_basis.r[2] = z_basis;

  XMStoreFloat3x3(&m_rotation, final_basis);
}

//=============================================================================
bool DirectSingleCube::IsPlacedOn(Dimention x, Dimention y, Dimention z) const
{
  return m_place.getX() == x && 
         m_place.getY() == y && 
         m_place.getZ() == z;
}
