#pragma once

#include "stdafx.h"


#include "..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\DirectXTK\Inc\Effects.h"

#include "RubicsCube\CustomPrimitiveBuilder.h"

class DirectCustomPrimitiveGeometry : public DirectX::GeometricPrimitive
{
public:
    
  static std::unique_ptr<DirectX::GeometricPrimitive> __cdecl CreateCubeAt(
    _In_ ID3D11DeviceContext* deviceContext, 
    DirectX::CXMVECTOR        centre, 
    float                     size, 
    float                     corner_rounding_coef,
    size_t                    tessellation, 
    const CubeColorsMap&      colors,
    bool                      rhcoords
  );

private:

  static const std::map<int, eCubeSide> facesMap;
};