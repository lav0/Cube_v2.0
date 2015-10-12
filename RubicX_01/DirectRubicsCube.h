#pragma once

#include "stdafx.h"
#include "DirectCubeFactory.h"

#include <vector>
#include <memory>
#include <algorithm>


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
    size_t a_tessellation,
    ID3D11DeviceContext* deviceContext,
    ID3D11ShaderResourceView* a_texture
  );

  virtual ~DirectRubicsCube() {}

  void Initialize();

  void Rotate(DirectX::CXMVECTOR a_quaternion);

  void TurnFace(const TurnCommand&);

  void Draw(DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection);

  void Update(float a_time_lapsed);

  const bool IsTurning() const;

  const Dimention GetDimention() const;
  
  static const float OuterSphereRadius(); 

protected:

  virtual void buildSubcubes();
  void alignSubcubes();

private:

  static const float bigcubeSize;
  static const float clearance;
  static const float turningSpeed;

  void beginTurning();
  void processTurning(float a_time_lapsed);
  void endTurning();

  void fillTurningCubesContrainer();

  float subcubeEdge() const;

  float deltaTurningAngle(float a_time_lapsed) const;

private:

  std::vector<std::shared_ptr<DirectSingleCube>> m_subcubes;

  DirectX::XMFLOAT3X3 m_position;

  const Dimention m_dimention;

  std::unique_ptr<DirectCubeFactory> m_unq_factory;
  
  TurnCommand m_cur_turning_command;
  size_t      m_turning_axis_index;
  Dimention   m_face_number;
  float       m_cur_turning_angle;
  float       m_aim_turning_angle;
  short       m_turning_direction;
  bool        m_b_turning;

  std::vector<std::shared_ptr<DirectSingleCube>> m_turning_cubes;
};