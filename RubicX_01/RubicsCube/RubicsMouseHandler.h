#pragma once

//#include "../stdafx.h"

#include "../../RubicMath/include/rcbSphere.h"
#include "../../RubicMath/include/rcbPlaneForScreen.h"

#include <cassert>

class RubicsMouseHandler
{
public:

  RubicsMouseHandler(
    const rcbVector3D& centre,
    const rcbVector3D& eye_direction,
    const rcbVector3D& eye_up_of_view,
    float eye_to_centre,
    float eye_to_screen,
    float a_radius,
    float proj0, float proj1
  );

  void Listen(bool a_pressed, float a_x, float a_y);

  rcbQuaternion Transformation() const;
  
  bool rotationStartIsZero() const;

private:

  bool findPointOnSphere(float a_x,
                         float a_y, 
                         rcbVector3D& a_output) const;
  
private:
  
  float             m_scaling_coef_x;
  float             m_scaling_coef_y;

  float             m_eye_to_screen_distance;
    
  rcbSphere         m_sphere;
  rcbVector3D       m_vc_eye;
  rcbPlaneForScreen m_screen_plane;
    
  rcbVector3D       m_rotation_start;
  rcbVector3D       m_rotation_axis;
  float             m_angle;
};