
#include "stdafx.h"
#include "MouseHandler.h"

using namespace DirectX;

//=============================================================================
//
//=============================================================================
MouseHandler::MouseHandler(
  const rcbVector3D& centre,
  const rcbVector3D& eye_direction,
  const rcbVector3D& eye_up_of_view,
  float eye_to_centre,
  float eye_to_screen,
  float a_radius,
  float proj0, float proj1
)
: m_eye_to_screen_distance(eye_to_screen)
, m_sphere(centre, a_radius)
, m_vc_eye(rcbVector3D(0, 0, 0) - eye_to_centre*eye_direction)
  , m_screen_plane(
  eye_direction,
  -1 * eye_up_of_view,
  m_vc_eye + eye_to_screen*eye_direction
    )
  , m_rotation_start(0.f, 0.f, 0.f)
  , m_rotation_axis(0.f, 0.f, 0.f)
  , m_angle(0.f)
//
//
//
{
  auto proj_11 = proj0;
  auto proj_22 = proj1;
  
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
  rcbVector3D point;

  if (
    a_pressed && 
    findPointOnSphere(-a_x * m_scaling_coef_x, a_y * m_scaling_coef_y, point)
  ) 
  {
    if (!rotationStartIsZero()) 
    {      
      auto& xvc0 = m_rotation_start;
      auto& xvc1 = point;

      m_rotation_axis = xvc0.vector_mul(xvc1);
      if (!m_rotation_axis.is_zero_vector())
        m_rotation_axis.normalize();

      m_angle = xvc0 ^ xvc1;
    }

    m_rotation_start = point;
  }
  else 
  {    
    m_rotation_start = rcbVector3D(0.f, 0.f, 0.f);

    m_angle = 0.0005f;
  }
}

//=============================================================================
rcbQuaternion MouseHandler::Transformation() const
//
//
//
{
  rcbQuaternion result(Rotation(rcbUnitVector3D::ort_z(), 0.f));

  if (is_zero_dbl(m_rotation_axis.square_norm() - 1))
  {
    result = rcbQuaternion(Rotation(m_rotation_axis, m_angle));
  }

  return result;
}

//=============================================================================
bool MouseHandler::findPointOnSphere(
  float a_x, 
  float a_y, 
  rcbVector3D& a_output
) const
//
//
//
{
  rcbVector3D vc_world(m_screen_plane.screenToWorld(a_x, a_y));

  rcbLine3D line(m_vc_eye, vc_world);
  
  if (m_sphere.intersection(line, vc_world))
  {
    a_output = vc_world;

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
  return m_rotation_start.square_norm() < 0.001;
}

//=============================================================================