
#include "stdafx.h"
#include "MouseHandler.h"

using namespace DirectX;

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
MouseHandler::MouseHandler(
  CXMMATRIX a_view, 
  CXMMATRIX a_proj,
  float a_eye_to_screen,
  CXMVECTOR a_centre, 
  float a_radius
)
  : m_eye_to_screen_distance(a_eye_to_screen)
  , m_sphere(convertToRCB(a_centre), a_radius)
  , m_vc_eye(rcbVector3D(0,0,0) - eyeToCentre(a_view)*eyeDirectionOfView(a_view))
  , m_screen_plane(
      eyeDirectionOfView(a_view), 
      -1 * eyeUpOfView(a_view), 
      m_vc_eye + a_eye_to_screen*eyeDirectionOfView(a_view)
    )
  , m_rotation_start(0.f, 0.f, 0.f)
  , m_rotation_axis(0.f, 0.f, 0.f)
  , m_angle(0.f)
//
//
//
{
  auto proj_11 = XMVectorGetX(a_proj.r[0]);
  auto proj_22 = XMVectorGetY(a_proj.r[1]);
  
  if (is_zero_dbl(proj_11)) {
    assert(false);
    proj_11 = 1.f;
  }
    
  if (is_zero_dbl(proj_22)) {
    assert(false);
    proj_22 = 1.f;
  }

  m_scaling_coef_x = 1 / proj_11;
  m_scaling_coef_y = 1 / proj_22;
}

//=============================================================================
void MouseHandler::Listen(bool a_pressed, float a_x, float a_y)
//
// 
//
{
  Vector3 point;

  if (
    a_pressed && 
    findPointOnSphere(-a_x * m_scaling_coef_x, a_y * m_scaling_coef_y, point)
  ) 
  {
    if (!rotationStartIsZero()) 
    {      
      auto xvc0 = XMLoadFloat3(&m_rotation_start);
      auto xvc1 = XMLoadFloat3(&point);

      m_rotation_axis = XMVector3Normalize(XMVector3Cross(xvc0, xvc1));
      m_angle = XMVectorGetX(XMVector3AngleBetweenVectors(xvc0, xvc1));
    }

    m_rotation_start = point;
  }
  else 
  {    
    m_rotation_start = Vector3(0.f, 0.f, 0.f);

    m_angle = 0.0005f;
  }
}

//=============================================================================
XMMATRIX MouseHandler::Transformation() const
//
//
//
{
  XMMATRIX result = XMMatrixIdentity();

  if (is_zero_dbl(m_rotation_axis.LengthSquared() - 1))
  {
    result = XMMatrixRotationAxis(m_rotation_axis, m_angle);
  }

  return result;
}

//=============================================================================
bool MouseHandler::findPointOnSphere(
  float a_x, 
  float a_y, 
  XMFLOAT3& a_output
) const
//
//
//
{
  rcbVector3D vc_world(m_screen_plane.screenToWorld(a_x, a_y));

  rcbLine3D line(m_vc_eye, vc_world);
  
  if (m_sphere.intersection(line, vc_world))
  {
    a_output.x = static_cast<float>(vc_world.getX());
    a_output.y = static_cast<float>(vc_world.getY());
    a_output.z = static_cast<float>(vc_world.getZ());

    return true;
  } 

  return false;
}

//=============================================================================
bool MouseHandler::rotationStartIsZero() const
//
//
//
{
  return m_rotation_start.LengthSquared() < 0.001;
}

//=============================================================================