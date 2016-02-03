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
  , m_quat_rotation(Rotation(rcbUnitVector3D(0, 0, 1), 0.0))
  , m_unq_quat_full(nullptr)
  , m_place(place)
{
}

//=============================================================================
void DirectSingleCube::Rotate(const rcbQuaternion& a_quaternion)
{
  m_quat_rotation = a_quaternion * m_quat_rotation;
}

//=============================================================================
void DirectSingleCube::Turn(eTurnAxis axis, eAngle angle, const rcbQuaternion& quat_full_turn)
{
  m_place.turn(axis, angle);

  m_unq_quat_full = std::make_unique<rcbQuaternion>(
    quat_full_turn * m_quat_rotation
  );
}

//=============================================================================
void DirectSingleCube::EndTurn(const rcbQuaternion& quat_full_turn)
//
// Align the single cube to the given position. 
// Used for making the Rubic's cube to be flat on each face
//   after rotation one of the faces.
//
{
  if (m_unq_quat_full)
  {
    m_quat_rotation = quat_full_turn * (*m_unq_quat_full.get());

    m_unq_quat_full.reset();
  }
}

//=============================================================================
void DirectSingleCube::Draw()
{
  m_effect_wrapper->SetRotation(m_quat_rotation);

  m_geometry->Draw(m_effect_wrapper.get());
}

//=============================================================================
bool DirectSingleCube::IsPlacedOn(Dimention x, Dimention y, Dimention z) const
{
  return m_place.getX() == x && 
         m_place.getY() == y && 
         m_place.getZ() == z;
}
