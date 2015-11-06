#include "stdafx.h"
#include "DirectCubeManager.h"

#include <d3d11_1.h>
#include <directxmath.h>

#include "..\DirectXTK\Inc\DDSTextureLoader.h"
#include "..\DirectXTK\Inc\VertexTypes.h"

#include "TimeMeasureDecorator.h"

#ifdef TIME_MEASURE
#define DirectRubicsCube TimeMeasureDirectRubicsCubeDecorator
#define DirectCubeFactory TimeMeasureDirectCubeFactoryDecorator
#endif


//=============================================================================
DirectCubeManager::DirectCubeManager()
  : m_p_texture(nullptr)
  , m_xvc_eye(initialEyePosition())
  , m_time_moment(0.f)
  , m_time_lapsed(0.f)
{
}

//=============================================================================
DirectCubeManager::~DirectCubeManager()
{
  if (m_p_texture) m_p_texture->Release();
}

//=============================================================================
HRESULT DirectCubeManager::Initialize( ID3D11DeviceContext* a_context, 
                                       Dimention a_cube_dimention, 
                                       size_t a_tesselation,
                                       UINT a_width, 
                                       UINT a_height
                                      )
{
  ID3D11Device* device;
  a_context->GetDevice(&device);
    
  HRESULT hr(S_OK);
  
  // Load the Texture
  hr = DirectX::CreateDDSTextureFromFile(
    device, 
    L"TextureGreen.dds", 
    nullptr, 
    &m_p_texture
  );  

  if (FAILED(hr)) return hr;
  
	m_xmx_projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, 
                                           a_width / (FLOAT)a_height, 
                                           1.f, 
                                           100.0f 
                                          );

  setUpCamera();
  
  setUpMouseHandler();

  auto factory = std::make_unique<DirectCubeFactory>(
    a_context, m_p_texture, a_cube_dimention, a_tesselation, m_xmx_view, m_xmx_projection
  );

  m_cube = std::make_unique<DirectRubicsCube>(
    a_cube_dimention,
    std::move(factory)
  );
  m_cube->Initialize();

  return S_OK;
}

//=============================================================================
void DirectCubeManager::tick()
//
//
//
{
  // Update our time
  static float t = 0.0f;
  
  {
    static DWORD dwTimeStart = 0;

    DWORD dwTimeCur = GetTickCount();

    if (dwTimeStart == 0)
        dwTimeStart = dwTimeCur;

    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  m_time_lapsed = t - m_time_moment;
  m_time_moment = t;
}

//=============================================================================
void DirectCubeManager::setUpCamera()
//
//
//
{
  // Initialize the view matrix
	
  int sign = 1;

  if (XMVectorGetByIndex(m_xvc_eye, 2) < 0)
    sign = -1;

  XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
  XMVECTOR Up = XMVectorSet( 0.0f, sign * 1.0f, 0.0f, 0.0f );
	m_xmx_view = XMMatrixLookAtLH( m_xvc_eye, At, Up );   
}

//=============================================================================
void DirectCubeManager::setUpMouseHandler()
{
  m_mouse_handler.reset(new MouseHandler(
    m_xmx_view, 
    m_xmx_projection,
    1.f, 
    DirectX::XMVectorZero(), 
    m_cube->OuterSphereRadius()
  ));
}

//=============================================================================
XMVECTOR DirectCubeManager::initialEyePosition()
{
  return XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );;
}

//=============================================================================
void DirectCubeManager::Render()
//
//
//
{
  tick();

  XMMATRIX localTransfrom = m_mouse_handler->Transformation();
  
  XMVECTOR xvc_quat_rotate = XMQuaternionRotationMatrix(localTransfrom);
  
  m_cube->Update(m_time_lapsed);
  m_cube->Rotate(xvc_quat_rotate);
  
  if (!m_cube->IsTurning() && !m_commands_sequence.empty())
  {
    m_cube->TurnFace(m_commands_sequence.front());
    m_commands_sequence.pop();
  }
        
  m_cube->Draw();

}

//--------------------------------------------------------------------------------------
void DirectCubeManager::ListenToMouse(bool a_pressed, float a_x, float a_y)
//
//
//
{
  m_mouse_handler->Listen(a_pressed, a_x, a_y);
}

//=============================================================================
void DirectCubeManager::ListenToKeyboard(short a_buttonPressed)
//
//
//
{  
  Dimention dim = m_cube->GetDimention();
  auto base = 3 * dim + 1;

  if (a_buttonPressed % base == 0)
    return;
  
  TurnCommand command;
  
  command.angle = A_90;
  
  // shift key handling  
  if (a_buttonPressed > base) {
    command.angle = A_270;
    a_buttonPressed -= base;
  }
  
  --a_buttonPressed;
  
  short axisId = a_buttonPressed / dim;

  command.axis = TA_X;

  if (axisId >= 1)
    command.axis = TA_Y;
  if (axisId >= 2)
    command.axis = TA_Z;

  command.face = a_buttonPressed - dim*axisId;

  m_commands_sequence.push(command);
}