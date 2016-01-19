#include "stdafx.h"
#include "DirectSingleCube.h"
#include "../DirectXTK/Src/PlatformHelpers.h"

using namespace DirectX;
using namespace Microsoft::WRL;

//=============================================================================
static void CreateInputLayout(_In_ ID3D11Device* device, IEffect* effect, _Outptr_ ID3D11InputLayout** pInputLayout)
{
  assert(pInputLayout != 0);

  void const* shaderByteCode;
  size_t byteCodeLength;

  effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

  ThrowIfFailed(
    device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
    VertexPositionNormalTexture::InputElementCount,
    shaderByteCode, byteCodeLength,
    pInputLayout)
    );

  //SetDebugObjectName(*pInputLayout, "DirectXTK:GeometricPrimitive");
}

//=============================================================================
//
//=============================================================================
DirectSingleCube::DirectSingleCube(
  ID3D11DeviceContext*       immediate_context,
  ID3D11ShaderResourceView*  texture,
  XMFLOAT3                   origin,
  const CubeColorsMap&       colors,
  const rcbCubePosition&     place,
  float                      size,
  size_t                     tessellation,
  DirectX::CXMMATRIX         view,
  DirectX::CXMMATRIX         projection
)
  : m_origin(origin)
  , m_size(size)
  , m_place(place)
{
  XMStoreFloat3x3(&m_rotation, XMMatrixIdentity());
  
  m_geometry = GeometricPrimitiveCustom::CreateCubeAt(
                                                immediate_context, 
                                                XMLoadFloat3(&m_origin), 
                                                m_size,
                                                0.2f,
                                                tessellation,
                                                colors,
                                                false
                                                );

  ComPtr<ID3D11Device> device;
  immediate_context->GetDevice(&device);

  m_effect = std::make_unique<BasicEffect>(device.Get());
  m_effect->EnableDefaultLighting();
  m_effect->SetTextureEnabled(true);
  ::CreateInputLayout(device.Get(), m_effect.get(), &m_input_layout);

  m_effect->SetTexture(texture);
  m_effect->SetView(view);
  m_effect->SetProjection(projection);

  setUpLighting();
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

  m_effect->SetWorld(pos);
  
  m_geometry->Draw(m_effect.get(), m_input_layout.Get());
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
  return m_place.getX() == x && m_place.getY() == y && m_place.getZ() == z;
}

//=============================================================================
void DirectSingleCube::setUpLighting()
{
  m_effect->SetLightEnabled(0, true);
  m_effect->SetLightEnabled(1, true);
  m_effect->SetLightEnabled(2, true);

  float lean_value = LIGHT_SOURCE_LEAN_FACTOR();

  float x0(.0), y0(.0), x1(.0), y1(.0), x2(.0), y2(.0);
  XMScalarSinCos(&x0, &y0, 0.0);
  XMScalarSinCos(&x1, &y1, 2 * XM_PI / 3);
  XMScalarSinCos(&x2, &y2, 4 * XM_PI / 3);

  float scale_factor = LIGHT_SOURCE_SCALE_FACTOR();

  XMVECTOR scale = XMVectorSet(
    scale_factor * lean_value,
    scale_factor * lean_value,
    scale_factor,
    .0f);

  XMVECTOR light0 = XMVectorMultiply(scale, XMVectorSet(x0, y0, 0.2f, .0f));
  XMVECTOR light1 = XMVectorMultiply(scale, XMVectorSet(x1, y1, 0.5f, .0f));
  XMVECTOR light2 = XMVectorMultiply(scale, XMVectorSet(x2, y2, 0.7f, .0f));

  m_effect->SetLightDirection(0, light0);
  m_effect->SetLightDirection(1, light1);
  m_effect->SetLightDirection(2, light2);
}