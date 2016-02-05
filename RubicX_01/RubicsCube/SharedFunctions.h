#pragma once

#include "SHARED_DEFS.h"
#include "../../RubicMath/include/rcbVector3D.h"
#include "../../RubicMath/include/rcbUnitVector3D.h"


namespace SharedFunctions
{
  rcbVector3D convertToRCB(const CALC_VECTOR4_DFN& vector);

  rcbUnitVector3D eyeDirectionOfView(const MATRIX4X4_DFN& view);

  rcbUnitVector3D eyeUpOfView(const MATRIX4X4_DFN& view);

  float eyeToCentre(const MATRIX4X4_DFN& view);

  MATRIX4X4_DFN castToMatrix(void* pBare);

  float scaleX(const MATRIX4X4_DFN& proj);

  float scaleY(const MATRIX4X4_DFN& proj);
}