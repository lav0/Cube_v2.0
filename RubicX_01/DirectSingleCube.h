#pragma once

#include "stdafx.h"

#include "GeometricPrimitiveCustom.h"

#include <directxmath.h>
#include <memory>

static const float rcb_to_float(eAngle a_e_angle)
{
  switch (a_e_angle)
  {
  case A_90 :  return DirectX::XM_PIDIV2;
  case A_180 : return DirectX::XM_PI;
  case A_270 : return DirectX::XM_PIDIV2;
  };
  _ASSERT(false);
  return 0.f;
}

enum eSingleCubePos
{
  SCP_NONE   = -1,
  SCP_CENTRE = 0,
  SCP_EDGE   = 1,
  SCP_CORNER = 2
};

class DirectSingleCube
{
public:

  DirectSingleCube(
    ID3D11DeviceContext*       immediate_context,
    ID3D11ShaderResourceView*  texture,
    DirectX::XMFLOAT3          origin,
    const CubeColorsMap&       colors,
    const rcbCubePosition&     place,
    float                      size,
    size_t                     tessellation
  );

  void Rotate(DirectX::CXMVECTOR a_quaternion);

  void Turn(eTurnAxis axis, eAngle angle);

  void Draw(DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection);

  void Align(DirectX::CXMMATRIX basis);

  bool IsPlacedOn(Dimention x, Dimention y, Dimention z) const;
  
private:

  std::unique_ptr<DirectX::GeometricPrimitive> m_geometry;
  
  ID3D11ShaderResourceView* m_texture;

  DirectX::XMFLOAT3   m_origin;
  DirectX::XMFLOAT3X3 m_rotation;
  float               m_size;
  
  rcbCubePosition     m_place;
};