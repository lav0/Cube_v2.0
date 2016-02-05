#pragma once

//#include "../stdafx.h"

#include "RubicsCube.h"
#include "RubicsMouseHandler.h"
#include "IKeyboardListener.h"

#include <queue>

static const float eyePosition[4] = { 0.0f, 0.0f, -5.0f, 0.0f };

class RubicsCubeManager : public IKeyboardListener, public IMouseListener
{
public:

  bool Initialize(std::unique_ptr<IFactory>&& renderer_factory,
                  Dimention a_cube_dimention, 
                  size_t a_tesselation);

  void Render(float tima_lapsed);
  
  void ListenToMouse(bool a_pressed, float a_x, float a_y) final override;
  
  void ListenToKeyboard(short a_buttonPressed) final override;

private:
  
  void setUpRubicsMouseHandler(IViewProjectionWrapper* view_projection);

private:
  
  std::unique_ptr<RubicsCube>    m_cube;
  std::unique_ptr<RubicsMouseHandler>  m_mouse_handler;  
  
  std::queue<TurnCommand>        m_commands_sequence;
};