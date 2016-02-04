#pragma once

#include "stdafx.h"

#include "RubicsCube.h"
#include "MouseHandler.h"
#include "IKeyboardListener.h"

#include <queue>

using namespace DirectX;

static const float eyePosition[4] = { 0.0f, 0.0f, -5.0f, 0.0f };

class RubicsCubeManager : public IKeyboardListener, public IMouseListener
{
public:

  RubicsCubeManager();

  bool Initialize(std::unique_ptr<IFactory>&& renderer_factory,
                  Dimention a_cube_dimention, 
                  size_t a_tesselation);

  void Render();
  
  void ListenToMouse(bool a_pressed, float a_x, float a_y) final override;
  
  void ListenToKeyboard(short a_buttonPressed) final override;

private:
  
  void tick();
  
  void setUpMouseHandler(IViewProjectionWrapper* view_projection);

private:
  
  std::unique_ptr<RubicsCube> m_cube;
  std::unique_ptr<MouseHandler>     m_mouse_handler;  
  
  std::queue<TurnCommand>           m_commands_sequence;
  
  float                             m_time_moment;
  float                             m_time_lapsed;
};