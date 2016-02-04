#include "stdafx.h"
#include "DirectViewProjectionWrapper.h"

//=============================================================================
DirectViewProjectionWrapper::DirectViewProjectionWrapper()
{
}

//=============================================================================
bool DirectViewProjectionWrapper::Initialize(unsigned int width,
  unsigned int height,
  const float* eye_position)
{
  auto eye = DirectX::XMVectorSet(eye_position[0],
    eye_position[1],
    eye_position[2],
    eye_position[3]);

  int sign = 1;

  if (DirectX::XMVectorGetByIndex(eye, 2) < 0)
    sign = -1;

  auto At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
  auto Up = DirectX::XMVectorSet(0.0f, sign * 1.0f, 0.0f, 0.0f);
  auto view = DirectX::XMMatrixLookAtLH(eye, At, Up);

  auto proj = DirectX::XMMatrixPerspectiveFovLH(fovAngle,
                                                width / (FLOAT)height,
                                                1.f,
                                                100.f
                                                );

  DirectX::XMStoreFloat4x4(&m_view, view);
  DirectX::XMStoreFloat4x4(&m_proj, proj);

  return true;
}

//=============================================================================
void* DirectViewProjectionWrapper::GetView() const
{
  return (void*)&m_view;
}
//=============================================================================
void* DirectViewProjectionWrapper::GetProjection() const
{
  return (void*)&m_proj;
}