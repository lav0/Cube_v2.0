#pragma once

#include "stdafx.h"

#include <directxmath.h>

#include "../DirectXTK/Inc/SimpleMath.h"

#include "../RubicMath/include/rcbSphere.h"
#include "../RubicMath/include/rcbPlaneForScreen.h"

using namespace DirectX::SimpleMath;

class MouseHandler
{
public:

  MouseHandler(
    DirectX::CXMMATRIX a_view,
    DirectX::CXMMATRIX a_proj,
    float a_eye_to_screen, 
    DirectX::CXMVECTOR a_centre, 
    float a_radius
  );

  void Listen(bool a_pressed, float a_x, float a_y);

  DirectX::XMMATRIX Transformation() const;
  
  bool rotationStartIsZero() const;

private:

  bool findPointOnSphere(float a_x, float a_y, DirectX::XMFLOAT3& a_output) const;
  
private:
  
  float             m_scaling_coef_x;
  float             m_scaling_coef_y;

  float             m_eye_to_screen_distance;
    
  rcbSphere         m_sphere;
  rcbVector3D       m_vc_eye;
  rcbPlaneForScreen m_screen_plane;
    
  Vector3           m_rotation_start;
  Vector3           m_rotation_axis;
  float             m_angle;
};