#pragma once

#include <windows.h>

class DirectTimer
{
public:

  float Tick();

private:

  float m_time_moment = 0.f;
  float m_time_lapsed = 0.f;

};