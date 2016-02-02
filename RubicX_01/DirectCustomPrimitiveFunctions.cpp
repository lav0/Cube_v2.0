#include "stdafx.h"
#include "CustomPrimitiveFunctions.h"

#include "..\DirectXTK\Inc\GeometricPrimitive.h"

using namespace DirectX;

XMVECTOR CustomPrimitiveFunctions::DotProduct(const XMVECTOR& v1, const XMVECTOR& v2)
{
  return XMVector4Dot(v1, v2);
}

XMVECTOR CustomPrimitiveFunctions::CrossProduct(const XMVECTOR& v1, const XMVECTOR& v2)
{
  return XMVector3Cross(v1, v2);
}

XMVECTOR CustomPrimitiveFunctions::VectorScale(const XMVECTOR& v, float factor)
{
  return XMVectorScale(v, factor);
}

XMVECTOR CustomPrimitiveFunctions::Normalize(const XMVECTOR& v)
{
  return XMVector4Normalize(v);
}

XMVECTOR CustomPrimitiveFunctions::QuaternionRotationAxis(const XMVECTOR& v, float angle)
{
  return XMQuaternionRotationAxis(v, angle);
}

XMVECTOR CustomPrimitiveFunctions::RotateWithQuaternion(const XMVECTOR& v, const XMVECTOR& quaternion)
{
  return XMVector3Rotate(v, quaternion);
}

XMVECTOR CustomPrimitiveFunctions::AngleBetweenNormals(const XMVECTOR& v1, const XMVECTOR& v2)
{
#ifdef _DEBUG

  auto SMALL_NUM = 0.0001f;

  auto x1 = GetX(v1);   auto x2 = GetX(v2);
  auto y1 = GetY(v1);   auto y2 = GetY(v2);
  auto z1 = GetZ(v1);   auto z2 = GetZ(v2);

  assert(fabs(x1*x1 + y1*y1 + z1*z1 - 1) < SMALL_NUM);
  assert(fabs(x2*x2 + y2*y2 + z2*z2 - 1) < SMALL_NUM);

#endif

  return XMVector3AngleBetweenNormals(v1, v2);
}

float CustomPrimitiveFunctions::GetX(const CALC_VECTOR4_DFN v)
{
  return XMVectorGetX(v);
}
float CustomPrimitiveFunctions::GetY(const CALC_VECTOR4_DFN v)
{
  return XMVectorGetY(v);
}
float CustomPrimitiveFunctions::GetZ(const CALC_VECTOR4_DFN v)
{
  return XMVectorGetZ(v);
}
float CustomPrimitiveFunctions::GetW(const CALC_VECTOR4_DFN v)
{
  return XMVectorGetW(v);
}
