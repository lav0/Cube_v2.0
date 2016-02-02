#include "stdafx.h"
#include "DirectCustomPrimitiveGeometry.h"

#include "../RubicMath/include/rcbVector3D.h"

#include "..\DirectXTK\Inc\VertexTypes.h"

#include <vector>

using namespace DirectX;


//=============================================================================
//
//=============================================================================
std::unique_ptr<DirectX::GeometricPrimitive> DirectCustomPrimitiveGeometry::CreateCubeAt(
  _In_ ID3D11DeviceContext* deviceContext, 
  CXMVECTOR                 centre, 
  float                     size, 
  float                     corner_rounding_coef,
  size_t                    tessellation, 
  const CubeColorsMap&      colors,
  bool                      rhcoords
)
{
  assert(corner_rounding_coef >= 0);
  assert(corner_rounding_coef < 1);
  
  CustomPrimitiveBuilder<VertexPositionNormalTexture, XMVECTOR> builder(
    tessellation, 
    corner_rounding_coef, 
    size, 
    colors
  );

  builder.CreateFaces(centre);
  builder.CreateEdges(centre);
  builder.CreateCorners(centre);

  return Initialize(deviceContext, 
                    builder.GetVertices(), 
                    builder.GetIndices(), 
                    rhcoords);
}


//=============================================================================