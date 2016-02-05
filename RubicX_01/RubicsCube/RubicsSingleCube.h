#pragma once

#include "IGeometry.h"
#include "IFactory.h"

const float M_PI      = 3.141592654f;
const float M_2PI     = 6.283185307f;
const float M_1DIVPI  = 0.318309886f;
const float M_1DIV2PI = 0.159154943f;
const float M_PIDIV2  = 1.570796327f;
const float M_PIDIV4  = 0.785398163f;

static const float rcb_to_float(eAngle a_e_angle)
{
  switch (a_e_angle)
  {
  case A_90:  return M_PIDIV2;
  case A_180: return M_PI;
  case A_270: return M_PIDIV2;
  };
  _ASSERT(false);
  return 0.f;
}

class RubicsSingleCube
{
public:

  RubicsSingleCube(
    std::unique_ptr<IGeometry>&&      geometry,
    std::unique_ptr<IEffectWrapper>&& effect_wrapper,
    const rcbCubePosition&            place
  );

  void Rotate(const rcbQuaternion& a_quaternion);

  void Turn(eTurnAxis            axis, 
            eAngle               angle, 
            const rcbQuaternion& quat_full_turn);

  void EndTurn(const rcbQuaternion& quat_full_turn);

  void Draw();

  bool IsPlacedOn(Dimention x, Dimention y, Dimention z) const;
    
private:

  std::unique_ptr<IGeometry> m_geometry;
  std::unique_ptr<IEffectWrapper> m_effect_wrapper;

  rcbQuaternion m_quat_rotation;

  std::unique_ptr<rcbQuaternion> m_unq_quat_full;
  
  rcbCubePosition     m_place;
};