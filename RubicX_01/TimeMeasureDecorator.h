#pragma once

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <string>

#include "DirectRubicsCube.h"
#include "DirectCubeFactory.h"

class TimeMeasureDirectRubicsCubeDecorator : public DirectRubicsCube
{
public:
  TimeMeasureDirectRubicsCubeDecorator(
    Dimention a_dimention,
    std::unique_ptr<DirectCubeFactory>&& unq_factory
  ) : DirectRubicsCube(a_dimention, std::move(unq_factory))
  {}


private:
  virtual void buildSubcubes() final override
  {
    const clock_t begin_time = clock();

    DirectRubicsCube::buildSubcubes();

    std::wstring str_time_lapse = std::to_wstring(float(clock() - begin_time) / CLOCKS_PER_SEC);
    OutputDebugString(L"Time lapsed for buildSubcubes() func: ");
    OutputDebugString(str_time_lapse.c_str());
    OutputDebugString(L"\n");
  }
};

class TimeMeasureDirectCubeFactoryDecorator : public DirectCubeFactory
{
public:

  TimeMeasureDirectCubeFactoryDecorator(
    ID3D11DeviceContext* deviceContext,
    ID3D11ShaderResourceView* a_texture,
    Dimention a_dimention,
    size_t a_tessellation,
    DirectX::CXMMATRIX view,
    DirectX::CXMMATRIX projection
  ) : DirectCubeFactory(deviceContext, a_texture, a_dimention, a_tessellation, view, projection) {}

  virtual std::unique_ptr<DirectSingleCube> CreateCube(
    const DirectX::XMFLOAT3&        origin,
    const float                     size,
    const std::array<Dimention, 3>&  cubeIndeces
  ) const final override
  {
    const clock_t begin_time = clock();

    auto result = DirectCubeFactory::CreateCube(origin, size, cubeIndeces);

    std::wstring str_time_lapse = std::to_wstring(float(clock() - begin_time) / CLOCKS_PER_SEC);
    OutputDebugString(L"Time lapsed for CreateCube() func: ");
    OutputDebugString(str_time_lapse.c_str());
    OutputDebugString(L"\n");

    return result;
  }

};