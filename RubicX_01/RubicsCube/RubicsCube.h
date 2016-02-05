#pragma once

//#include "../stdafx.h"

#include <vector>
#include <array>
#include <memory>
#include <algorithm>

#include "RubicsCubeFactory.h"

struct TurnCommand {
  eTurnAxis axis;
  Dimention face;
  eAngle    angle;
};


class RubicsCube
{
public:

  RubicsCube(
    Dimention a_dimention,
    std::vector<std::shared_ptr<RubicsSingleCube>> single_cubes
  );

  virtual ~RubicsCube() {}

  void Rotate(const rcbQuaternion& a_quaternion);

  void TurnFace(const TurnCommand&);

  void Draw();

  void Update(float a_time_lapsed);

  const bool IsTurning() const;

  const Dimention GetDimention() const;
  
  static const float RubicsCubeSize();
  static const float OuterSphereRadius(); 
  static const float Clearance();

protected:

  void alignSubcubes();

private:

  static const float bigcubeSize;
  static const float clearance;
  static const float turningSpeed;

  void beginTurning();
  void processTurning(float a_time_lapsed);
  void endTurning();

  void fillTurningCubesContrainer();

  float turningContinuousAngle(float a_time_lapsed) const;
  float turningStepAngle() const;
  
private:

  std::vector<std::shared_ptr<RubicsSingleCube>> m_subcubes;

  rcbQuaternion m_quat_position;

  std::array<rcbUnitVector3D, 3> m_cube_orts;

  const Dimention m_dimention;

  /////////////////////////////////////////////////////////////////////////////
  //    Face turning processing members
  /////////////////////////////////////////////////////////////////////////////

  rcbQuaternion m_quat_rotation_during_turning;

  TurnCommand m_cur_turning_command;
  size_t      m_turning_axis_index = 0;
  Dimention   m_face_number        = 0;
  float       m_cur_turning_angle  = 0.f;
  float       m_aim_turning_angle  = 0.f;
  short       m_turning_direction  = 1;
  bool        m_b_turning          = false;

  std::vector<std::shared_ptr<RubicsSingleCube>> m_turning_cubes;
};