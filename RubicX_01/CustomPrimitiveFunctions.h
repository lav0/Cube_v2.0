#pragma once

#include "SHARED_DEFS.h"


namespace CustomPrimitiveFunctions
{
  CALC_VECTOR4_DFN DotProduct(const CALC_VECTOR4_DFN& v1, const CALC_VECTOR4_DFN& v2);
  CALC_VECTOR4_DFN CrossProduct(const CALC_VECTOR4_DFN& v1, const CALC_VECTOR4_DFN& v2);
  CALC_VECTOR4_DFN VectorScale(const CALC_VECTOR4_DFN& v, float factor);
  CALC_VECTOR4_DFN Normalize(const CALC_VECTOR4_DFN& v);
  CALC_VECTOR4_DFN QuaternionRotationAxis(const CALC_VECTOR4_DFN& v, float angle);
  CALC_VECTOR4_DFN RotateWithQuaternion(const CALC_VECTOR4_DFN& v, const CALC_VECTOR4_DFN& quaternion);
  CALC_VECTOR4_DFN AngleBetweenNormals(const CALC_VECTOR4_DFN& v1, const CALC_VECTOR4_DFN& v2);

  float GetX(const CALC_VECTOR4_DFN v);
  float GetY(const CALC_VECTOR4_DFN v);
  float GetZ(const CALC_VECTOR4_DFN v);
}