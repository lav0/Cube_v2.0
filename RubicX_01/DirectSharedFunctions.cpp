#include "stdafx.h"
#include "RubicsCube\SharedFunctions.h"

#include <DirectXMath.h>

using namespace DirectX;

rcbVector3D SharedFunctions::convertToRCB(const XMVECTOR& vector)
{
  return rcbVector3D(
    XMVectorGetX(vector),
    XMVectorGetY(vector),
    XMVectorGetZ(vector)
  );
}

rcbUnitVector3D SharedFunctions::eyeDirectionOfView(const XMMATRIX& view)
{
  return convertToRCB(view.r[2]);
}

rcbUnitVector3D SharedFunctions::eyeUpOfView(const XMMATRIX& view)
{
  return convertToRCB(view.r[1]);
}

float SharedFunctions::eyeToCentre(const XMMATRIX& view)
{
  return XMVectorGetZ(view.r[3]);
}

XMMATRIX SharedFunctions::castToMatrix(void* pBare)
{
  return XMLoadFloat4x4(static_cast<XMFLOAT4X4*>(pBare));
}

float SharedFunctions::scaleX(const XMMATRIX& proj)
{
  return XMVectorGetX(proj.r[0]);
}

float SharedFunctions::scaleY(const XMMATRIX& proj)
{
  return XMVectorGetY(proj.r[1]);
}