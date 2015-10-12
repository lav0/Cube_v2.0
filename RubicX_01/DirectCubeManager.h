#pragma once

#include "stdafx.h"

#include "DirectRubicsCube.h"
#include "MouseHandler.h"
#include "IKeyboardListener.h"

#include "..\DirectXTK\Inc\Effects.h"

#include <queue>

using namespace DirectX;

class DirectCubeManager : public IKeyboardListener, public IMouseListener
{
public:

  DirectCubeManager();

  ~DirectCubeManager();

  HRESULT Initialize(ID3D11DeviceContext* a_context, Dimention a_cube_dimention, size_t a_tesselation, UINT a_screen_width, UINT a_screen_height);

  void Render();
  
  virtual void ListenToMouse(bool a_pressed, float a_x, float a_y) final override;
  
  virtual void ListenToKeyboard(short a_buttonPressed) final override;

private:
  
  void tick();
  
  void setUpCamera();

  void setUpMouseHandler();

  static XMVECTOR initialEyePosition();

private:
  
  ID3D11ShaderResourceView*         m_p_texture;
  
  std::unique_ptr<DirectRubicsCube> m_cube;
  std::unique_ptr<MouseHandler>     m_mouse_handler;  
  
  std::queue<TurnCommand>           m_commands_sequence;
  
  XMMATRIX                          m_xmx_view;
  XMMATRIX                          m_xmx_projection;

  XMVECTOR                          m_xvc_eye;

  float                             m_time_moment;
  float                             m_time_lapsed;
};