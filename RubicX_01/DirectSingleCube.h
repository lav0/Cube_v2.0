#pragma once

#include "stdafx.h"

#include "DirectGeometry.h"
#include "IFactory.h"

//#include "../RubicMath/include/rcbQuaternion.h"

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
    std::unique_ptr<IGeometry>&&      geometry,
    std::unique_ptr<IEffectWrapper>&& effect_wrapper,
    const rcbCubePosition&            place
  );

  void Rotate(DirectX::CXMVECTOR a_quaternion);

  void Turn(eTurnAxis axis, eAngle angle);

  void Draw();

  void Align(DirectX::CXMMATRIX basis);

  bool IsPlacedOn(Dimention x, Dimention y, Dimention z) const;
    
private:

  std::unique_ptr<IGeometry> m_geometry;
  std::unique_ptr<IEffectWrapper> m_effect_wrapper;

  DirectX::XMFLOAT3X3 m_rotation;

  //rcbQuaternion m_quat_rotation;
  
  rcbCubePosition     m_place;
};