#pragma once

#include "stdafx.h"


#include "..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\DirectXTK\Inc\Effects.h"

#include "../RubicMath/include/rcbCubeState.h"

#include <map>
#include <algorithm>

enum eCubeColor
{
  CC_BLACK     = 0,
  CC_GREEN     = 4,
  CC_RED       = 8,
  CC_BLUE      = 12,
  CC_ORANGE    = 16,
  CC_YELLOW    = 20,
  CC_TURQUOISE = 24
};

typedef std::map<eCubeSide, eCubeColor> CubeColorsMap;

class GeometricPrimitiveCustom : public DirectX::GeometricPrimitive
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