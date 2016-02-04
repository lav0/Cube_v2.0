#pragma once

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <string>

#include "RubicsCube.h"
#include "RubicsCubeFactory.h"

//class TimeMeasureDirectCubeFactoryDecorator : public RubicsCubeFactory
//{
//public:
//
//  TimeMeasureRubicsCubeFactoryDecorator(
//    ID3D11DeviceContext* deviceContext,
//    Dimention a_dimention,
//    size_t a_tessellation,
//    DirectX::CXMMATRIX view,
//    DirectX::CXMMATRIX projection
//  ) : RubicsCubeFactory(deviceContext, a_dimention, a_tessellation, view, projection) {}
//
//  virtual std::unique_ptr<RubicsSingleCube> CreateCube(
//    const DirectX::XMFLOAT3&        origin,
//    const float                     size,
//    const std::array<Dimention, 3>&  cubeIndeces
//  ) const final override
//  {
//    const clock_t begin_time = clock();
//
//    auto result = RubicsCubeFactory::CreateCube(origin, size, cubeIndeces);
//
//    std::wstring str_time_lapse = std::to_wstring(float(clock() - begin_time) / CLOCKS_PER_SEC);
//    OutputDebugString(L"Time lapsed for CreateCube() func: ");
//    OutputDebugString(str_time_lapse.c_str());
//    OutputDebugString(L"\n");
//
//    return result;
//  }
//
//};