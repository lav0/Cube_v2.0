#pragma once

#include "stdafx.h"

#include "GeometricPrimitiveCustom.h"

#include <directxmath.h>
#include <memory>
#include <wrl.h>


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

static const float LIGHT_SOURCE_LEAN_FACTOR() { return 0.6f; }
static const float LIGHT_SOURCE_SCALE_FACTOR() { return 0.6f; }

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
    size_t                     tessellation,
    DirectX::CXMMATRIX         view,
    DirectX::CXMMATRIX         projection
  );

  void Rotate(DirectX::CXMVECTOR a_quaternion);

  void Turn(eTurnAxis axis, eAngle angle);

  void Draw();

  void Align(DirectX::CXMMATRIX basis);

  bool IsPlacedOn(Dimention x, Dimention y, Dimention z) const;

private:

  void setUpLighting();
  
private:

  std::unique_ptr<DirectX::GeometricPrimitive> m_geometry;
  std::unique_ptr<DirectX::BasicEffect> m_effect;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_input_layout;

  DirectX::XMFLOAT3   m_origin;
  DirectX::XMFLOAT3X3 m_rotation;
  float               m_size;
  
  rcbCubePosition     m_place;
};