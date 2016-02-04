#pragma once



#ifdef DEPRECATED

#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#else

#include <d3d11_1.h>
#include <directxmath.h>

#include "..\DirectXTK\Inc\CommonStates.h"
#include "..\DirectXTK\Inc\DDSTextureLoader.h"
#include "..\DirectXTK\Inc\Effects.h"
#include "..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\DirectXTK\Inc\Model.h"
#include "..\DirectXTK\Inc\PrimitiveBatch.h"
#include "..\DirectXTK\Inc\ScreenGrab.h"
#include "..\DirectXTK\Inc\SpriteBatch.h"
#include "..\DirectXTK\Inc\SpriteFont.h"
#include "..\DirectXTK\Inc\VertexTypes.h"

#include "MouseHandler.h"

using namespace DirectX;

#endif

#include <atlcomcli.h>

#include <vector>
#include <queue>
#include <memory>
#include <algorithm>


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

class DirectCore
{
public:

  DirectCore(HWND&);

  virtual ~DirectCore();
  
  HRESULT InitDevice();

  void Clear();

  void SwapChain();

  ID3D11DeviceContext* GetImmediateContext() const;

private:

  HWND& m_ref_hwnd;

  CComPtr<ID3D11DeviceContext>    m_cp_immediate_context;
  CComPtr<IDXGISwapChain>         m_cp_swap_chain;  
  
  ID3D11InputLayout*              m_p_layout; 
  ID3D11RenderTargetView*         m_p_render_target_view;

  ID3D11DepthStencilView*         m_depth_stencil_view;
};
