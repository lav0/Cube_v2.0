#pragma once

#ifdef RENDER_DRX
#include "..\DirectXTK\Inc\GeometricPrimitive.h"
#define MATRIX4X4_DFN      DirectX::XMMATRIX
#define STORE_VECTOR3_DFN  DirectX::XMFLOAT3
#define CALC_VECTOR4_DFN   DirectX::XMVECTOR
#define ALIGNED_VECTOR_DFN DirectX::XMVECTORF32
#endif
