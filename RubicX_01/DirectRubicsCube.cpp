#include "stdafx.h"
#include "DirectRubicsCube.h"

#include "TimeMeasureDecorator.h"

using namespace DirectX;

const float DirectRubicsCube::bigcubeSize  = 4.f;
const float DirectRubicsCube::clearance    = 0.16f;
const float DirectRubicsCube::turningSpeed = 4.2f;

const rcbQuaternion staticQuaternion(Rotation(rcbUnitVector3D::ort_z(), 0.0));

//=============================================================================
DirectRubicsCube::DirectRubicsCube(
  Dimention a_dimention,
  std::vector<std::shared_ptr<DirectSingleCube>> single_cubes
 )
 : m_subcubes(std::move(single_cubes))
 , m_quat_position(staticQuaternion)
 , m_cube_orts{{ 
    rcbUnitVector3D::ort_x(), 
    rcbUnitVector3D::ort_y(), 
    rcbUnitVector3D::ort_z()
  }}
 , m_dimention(a_dimention)
 , m_quat_rotation_during_turning(staticQuaternion)
{  
  m_turning_cubes.clear();
}

//=============================================================================
void DirectRubicsCube::alignSubcubes()
{ 
  for (auto& subcube : m_turning_cubes)
  {
    subcube->EndTurn(m_quat_rotation_during_turning);
  }
}

//=============================================================================
void DirectRubicsCube::Rotate(const rcbQuaternion& a_quaternion)
{
  rcbQuaternion quat_arg(a_quaternion);

  m_quat_position = quat_arg * m_quat_position;

  for (auto& v : m_cube_orts)
  {
    v = quat_arg.turn(v);
  }

  std::for_each(m_subcubes.begin(), m_subcubes.end(), 
    [&quat_arg](std::shared_ptr<DirectSingleCube> cube)
    {
      cube->Rotate(quat_arg);
    }
  );

  if (m_b_turning)
  {
    m_quat_rotation_during_turning = quat_arg * m_quat_rotation_during_turning;
  }
}

//=============================================================================
void DirectRubicsCube::TurnFace(const TurnCommand& a_turn_command)
{
  _ASSERT(!m_b_turning);

  m_cur_turning_command = a_turn_command;
  
  beginTurning();
}

//=============================================================================
void DirectRubicsCube::Draw()
{  
  std::for_each(m_subcubes.begin(), m_subcubes.end(), 
    [](std::shared_ptr<DirectSingleCube> cube)
    {
      cube->Draw();
    }
  );
}

//=============================================================================
void DirectRubicsCube::Update(float a_time_lapsed)
{
  if (m_b_turning) 
  {
    processTurning(a_time_lapsed);

    endTurning();
  }
}

//=============================================================================
void DirectRubicsCube::beginTurning()
{
  if (m_b_turning)
    return;
  
  _ASSERT(m_cur_turning_command.face < m_dimention);
  
  eTurnAxis& e_axis  = m_cur_turning_command.axis;
  eAngle&    e_angle = m_cur_turning_command.angle;

  m_turning_axis_index = e_axis == TA_X ? 0 :
                        (e_axis == TA_Y ? 1 : 2);

  m_face_number        = m_cur_turning_command.face;
  m_aim_turning_angle = rcb_to_float(e_angle);
  m_turning_direction = e_angle == A_270 ? -1 : 1;

  fillTurningCubesContrainer();

  float delta = turningStepAngle();
  auto axis = m_cube_orts[m_turning_axis_index];
  rcbQuaternion quat_turn(
    Rotation(axis, delta)
  );

  for (auto& subcube : m_turning_cubes)
  {
    subcube->Turn(e_axis, e_angle, quat_turn);
  }

  m_b_turning = true;  
}

//=============================================================================
void DirectRubicsCube::processTurning(float a_time_lapsed)
//
// 
//
{
  _ASSERT(m_turning_axis_index >= 0 && m_turning_axis_index <= 2);

  auto delta = turningContinuousAngle(a_time_lapsed);
  auto axis = m_cube_orts[m_turning_axis_index];

  rcbQuaternion quat_arg(
    Rotation(axis, delta)
  );
  
  std::for_each(m_turning_cubes.begin(), m_turning_cubes.end(),
    [&quat_arg](std::shared_ptr<DirectSingleCube>& subcube)
    {
      subcube->Rotate(quat_arg);
    }
  );

  m_cur_turning_angle += delta;
}

//=============================================================================
void DirectRubicsCube::endTurning()
{
  if (abs(m_cur_turning_angle) < m_aim_turning_angle)
    return;

  alignSubcubes();

  m_turning_cubes.clear();
  
  m_cur_turning_angle = 0.f;
  m_turning_direction = 0;

  m_quat_rotation_during_turning = staticQuaternion;

  m_b_turning = false;
}

//=============================================================================
void DirectRubicsCube::fillTurningCubesContrainer()
{
  _ASSERT(m_turning_cubes.empty());
    
  Dimention i(0), j(0);
  Dimention *fixed_i(&i), *fixed_j(&j), *fixed_k(&m_face_number);

  switch (m_turning_axis_index) {
  case 0 :
    fixed_i = &m_face_number;
    fixed_j = &i;
    fixed_k = &j;
    break;
  case 1 :
    fixed_i = &i;
    fixed_j = &m_face_number;
    fixed_k = &j;
    break;
  };
  
  std::for_each(m_subcubes.begin(), m_subcubes.end(), 
    [&fixed_i, &fixed_j, &fixed_k, &i, &j, this](std::shared_ptr<DirectSingleCube> cube)
    {
      for (i=0; i<m_dimention; ++i) {
        for (j=0; j<m_dimention; ++j)
        {
          if (cube->IsPlacedOn(*fixed_i, *fixed_j, *fixed_k)) {
            m_turning_cubes.push_back(cube);
            i = m_dimention;
            j = m_dimention;
          }
        }
      }
    }
  );
}

//=============================================================================
const float DirectRubicsCube::OuterSphereRadius()
{
  return (bigcubeSize / 2) * sqrtf(3.0);
}

//=============================================================================
const float DirectRubicsCube::RubicsCubeSize()
{
  return bigcubeSize;
}

//=============================================================================
const float DirectRubicsCube::Clearance()
{
  return clearance;
}

//=============================================================================
const bool DirectRubicsCube::IsTurning() const
{
  return m_b_turning;
}

//=============================================================================
const Dimention DirectRubicsCube::GetDimention() const
{
  return m_dimention;
}

//=============================================================================
float DirectRubicsCube::turningContinuousAngle(float a_time_lapsed) const
{
  return m_turning_direction * turningSpeed * a_time_lapsed;
}

//=============================================================================
float DirectRubicsCube::turningStepAngle() const
{
  return m_turning_direction * m_aim_turning_angle; // expect M_PI_2
}
//=============================================================================