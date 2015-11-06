#include "stdafx.h"
#include "DirectRubicsCube.h"

#include "TimeMeasureDecorator.h"

using namespace DirectX;

const float DirectRubicsCube::bigcubeSize  = 4.f;
const float DirectRubicsCube::clearance    = 0.16f;
const float DirectRubicsCube::turningSpeed = 3.f;

//=============================================================================
DirectRubicsCube::DirectRubicsCube(
  Dimention a_dimention,
  std::unique_ptr<DirectCubeFactory>&& unq_factory
 )
  : m_position(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f)
  , m_dimention(a_dimention)
  , m_unq_factory(std::move(unq_factory))
{
}

//=============================================================================
void DirectRubicsCube::Initialize()
{
  m_turning_cubes.reserve(m_dimention * m_dimention);

  try
  {
    buildSubcubes();
  }
  catch (std::exception& e)
  {
    if (m_unq_factory->DecreaseTessellation())
    {
      std::for_each(m_subcubes.begin(), m_subcubes.end(), 
        [](std::shared_ptr<DirectSingleCube> cube)
        {
          cube.reset();
        }
      );
      Initialize();
    }
    else
    {
      throw e;
    }
  }
}

//=============================================================================
void DirectRubicsCube::buildSubcubes()
{
  float d_long = bigcubeSize / m_dimention;
  float d_edge = subcubeEdge();
  float d_half = bigcubeSize / 2;
  
  m_subcubes.reserve(m_dimention * m_dimention * m_dimention);

  for (Dimention i = 0; i < m_dimention; ++i) {
    for (Dimention j = 0; j < m_dimention; ++j) {
      for (Dimention k = 0; k < m_dimention; ++k) 
      {
        DirectX::XMFLOAT3 vc_centre(
          d_long/2 + i * d_long - d_half,
          d_long/2 + j * d_long - d_half,
          d_long/2 + k * d_long - d_half
        );

        float size = bigcubeSize;
        
        const std::array<Dimention,3> indeces = {i, j, k};

        m_subcubes.push_back(m_unq_factory->CreateCube( vc_centre, 
                                                        d_edge, 
                                                        indeces
                                                       ));
      }
    }
  }
}

//=============================================================================
void DirectRubicsCube::alignSubcubes()
{ 
  XMMATRIX pos = XMLoadFloat3x3(&m_position);

  std::for_each(m_turning_cubes.begin(), m_turning_cubes.end(), 
    [&pos](std::shared_ptr<DirectSingleCube> cube)
    {
      cube->Align(pos);
    }
  );
}

//=============================================================================
void DirectRubicsCube::Rotate(CXMVECTOR a_quaternion)
{
  XMMATRIX position = XMLoadFloat3x3(&m_position);
  XMMATRIX rotation = XMMatrixRotationQuaternion(a_quaternion);

  position = position * rotation;

  XMStoreFloat3x3(&m_position, position);

  std::for_each(m_subcubes.begin(), m_subcubes.end(), 
    [&a_quaternion](std::shared_ptr<DirectSingleCube> cube)
    {
      cube->Rotate(a_quaternion);
    }
  );
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
  
  eTurnAxis& axis  = m_cur_turning_command.axis;
  eAngle&    angle = m_cur_turning_command.angle;

  m_turning_axis_index = axis == TA_X ? 0 : 
                        (axis == TA_Y ? 1 : 2);

  m_face_number        = m_cur_turning_command.face;
  m_aim_turning_angle  = rcb_to_float(angle);
  m_turning_direction  = angle == A_270 ? -1 : 1;

  fillTurningCubesContrainer();
  
  std::for_each(m_turning_cubes.begin(), m_turning_cubes.end(),
    [&axis, &angle](std::shared_ptr<DirectSingleCube>& subcube)
    {
      subcube->Turn(axis, angle);
    }
  );

  m_b_turning = true;  
}

//=============================================================================
void DirectRubicsCube::processTurning(float a_time_lapsed)
//
// 
//
{
  _ASSERT(m_turning_axis_index >= 0 && m_turning_axis_index <= 2);

  float delta = deltaTurningAngle(a_time_lapsed);

  XMVECTOR quaternion = XMQuaternionRotationAxis(
    XMLoadFloat3x3(&m_position).r[m_turning_axis_index], 
    delta
  );
  
  std::for_each(m_turning_cubes.begin(), m_turning_cubes.end(),
    [&quaternion](std::shared_ptr<DirectSingleCube>& subcube)
    {
      subcube->Rotate(quaternion);
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
float DirectRubicsCube::subcubeEdge() const
{
  return (bigcubeSize - clearance) / m_dimention;
}

//=============================================================================
const float DirectRubicsCube::OuterSphereRadius()
{
  return (bigcubeSize / 2) * sqrtf(3.0);
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
float DirectRubicsCube::deltaTurningAngle(float a_time_lapsed) const
{
  return m_turning_direction * turningSpeed * a_time_lapsed;
}
//=============================================================================