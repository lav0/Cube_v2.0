#include "stdafx.h"
#include "DirectCubeManager.h"

#include <d3d11_1.h>
#include <directxmath.h>

#include "..\DirectXTK\Inc\VertexTypes.h"
#include "DirectFactory.h"

#include "RubicsCubeBuilder.h"
#include "TimeMeasureDecorator.h"

#ifdef TIME_MEASURE
#define DirectCubeFactory TimeMeasureDirectCubeFactoryDecorator
#endif

//=============================================================================
rcbVector3D convertToRCB(CXMVECTOR vector)
{
  return rcbVector3D(
    XMVectorGetX(vector),
    XMVectorGetY(vector),
    XMVectorGetZ(vector)
    );
}

rcbUnitVector3D eyeDirectionOfView(CXMMATRIX view)
{
  return convertToRCB(view.r[2]);
}

rcbUnitVector3D eyeUpOfView(CXMMATRIX view)
{
  return convertToRCB(view.r[1]);
}

float eyeToCentre(CXMMATRIX view)
{
  return XMVectorGetZ(view.r[3]);
}


//=============================================================================
//
//=============================================================================
DirectCubeManager::DirectCubeManager()
  : m_xvc_eye(initialEyePosition())
  , m_time_moment(0.f)
  , m_time_lapsed(0.f)
{
}

//=============================================================================
DirectCubeManager::~DirectCubeManager()
{
}

//=============================================================================
HRESULT DirectCubeManager::Initialize( ID3D11DeviceContext* a_context, 
                                       Dimention a_cube_dimention, 
                                       size_t a_tesselation,
                                       UINT a_width, 
                                       UINT a_height
                                      )
{
	m_xmx_projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, 
                                           a_width / (FLOAT)a_height, 
                                           1.f, 
                                           100.0f 
                                          );

  setUpCamera();
  
  setUpMouseHandler();

  auto texture_holder = std::make_unique<DirectTextureHolder>(a_context);

  auto geometry_factory = std::make_unique<DirectFactory>(
    a_context, texture_holder->GetTexture());

  auto factory = std::make_unique<DirectCubeFactory>(
    std::move(geometry_factory),
    a_cube_dimention, 
    a_tesselation, 
    m_xmx_view, 
    m_xmx_projection
  );

  RubicsCubeBuilder builder(a_cube_dimention, std::move(factory));

  m_cube = builder.build_cube();

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
  auto centre = rcbVector3D(0.f, 0.f, 0.f);
  auto eye_to_centre = eyeToCentre(m_xmx_view);
  auto eye_direction = eyeDirectionOfView(m_xmx_view);
  auto eye_up_of_view = eyeUpOfView(m_xmx_view);

  m_mouse_handler.reset(new MouseHandler(
    centre,
    eye_direction,
    eye_up_of_view,
    eye_to_centre,
    1.f, 
    m_cube->OuterSphereRadius(),
    XMVectorGetX(m_xmx_projection.r[0]),
    XMVectorGetY(m_xmx_projection.r[1])
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

  //auto localTransfrom = m_mouse_handler->Transformation();
  
  auto quat_rotate = m_mouse_handler->Transformation();
  
  m_cube->Update(m_time_lapsed);
  m_cube->Rotate(quat_rotate);
  
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