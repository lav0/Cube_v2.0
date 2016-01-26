#pragma once

#include "stdafx.h"

#include <vector>
#include <memory>
#include <algorithm>

#include "DirectCubeFactory.h"

struct TurnCommand {
  eTurnAxis axis;
  Dimention face;
  eAngle    angle;
};


class DirectRubicsCube
{
public:

  DirectRubicsCube(
    Dimention a_dimention,
    std::vector<std::shared_ptr<DirectSingleCube>> single_cubes
  );

  virtual ~DirectRubicsCube() {}

  void Rotate(DirectX::CXMVECTOR a_quaternion);

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

  float deltaTurningAngle(float a_time_lapsed) const;

private:

  std::vector<std::shared_ptr<DirectSingleCube>> m_subcubes;

  DirectX::XMFLOAT3X3 m_position;

  const Dimention m_dimention;

  TurnCommand m_cur_turning_command;
  size_t      m_turning_axis_index = 0;
  Dimention   m_face_number        = 0;
  float       m_cur_turning_angle  = 0.f;
  float       m_aim_turning_angle  = 0.f;
  short       m_turning_direction  = 1;
  bool        m_b_turning          = false;

  std::vector<std::shared_ptr<DirectSingleCube>> m_turning_cubes;
};