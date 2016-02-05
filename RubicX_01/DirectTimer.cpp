#include "stdafx.h"
#include "DirectTimer.h"

//=============================================================================
float DirectTimer::Tick()
{
  static float t = 0.0f;

  {
    static unsigned long dwTimeStart = 0;

    auto dwTimeCur = GetTickCount();

    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;

    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  m_time_lapsed = t - m_time_moment;
  m_time_moment = t;

  return m_time_lapsed;
}