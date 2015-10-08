#pragma once

#include "stdafx.h"

enum eCubeSide {
  CS_DOWN = 1,
  CS_UP   = 2,
  CS_BACK = 3, 
  CS_FRONT = 4,
  CS_RIGHT = 5,
  CS_LEFT = 6
};

enum eTurnAxis
{
  TA_X = 0,
  TA_Y = 1,
  TA_Z = 2
};

enum eAngle
{
  A_90 = 0,
  A_180 = 1,
  A_270 = 2
};

typedef unsigned short Dimention;

bool operator==(eTurnAxis, eCubeSide);
eCubeSide opposite(eCubeSide);
eCubeSide turn_rules(eCubeSide, eTurnAxis, eAngle);

class rcbCubeState
{
public:
  rcbCubeState();
  rcbCubeState(eCubeSide a_z, eCubeSide a_x);

  void turn(eTurnAxis, eAngle);

  eCubeSide get_z_side() const { return m_z_side; }
  eCubeSide get_x_side() const { return m_x_side; }

private:

  eCubeSide m_z_side;
  eCubeSide m_x_side;
};

class rcbCubePosition
{
public:
  rcbCubePosition(Dimention size, Dimention x ,Dimention y, Dimention z);

  void turn(eTurnAxis, eAngle);

  Dimention getX() const { return m_pos_x; };
  Dimention getY() const { return m_pos_y; };
  Dimention getZ() const { return m_pos_z; };

private:

  Dimention m_size;
  Dimention m_pos_x;
  Dimention m_pos_y;
  Dimention m_pos_z;
};