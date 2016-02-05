#pragma once

struct IViewProjectionWrapper
{
  virtual ~IViewProjectionWrapper() {}

  virtual bool Initialize(unsigned int width,
    unsigned int height,
    const float* eye_position) = 0;

  virtual void* GetView() const = 0;
  virtual void* GetProjection() const = 0;
};