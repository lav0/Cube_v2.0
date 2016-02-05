
#include "DirectCore.h"

#define EYE_TO_SCREEN 0.3f

//=============================================================================
// MEMBER FUNCTIONS GO FURTHER
//=============================================================================



//=============================================================================
DirectCore::DirectCore(HWND& a_hwnd)
  : m_ref_hwnd(a_hwnd)
  , m_p_layout(nullptr)
  , m_p_render_target_view(nullptr)
  , m_depth_stencil_view(nullptr)
//
//
//
{
}


//=============================================================================
DirectCore::~DirectCore()
//
//
//
{ 
  if (m_p_render_target_view) m_p_render_target_view->Release(); 
  if (m_p_layout)             m_p_layout->Release(); 
  if (m_depth_stencil_view)   m_depth_stencil_view->Release();
}


//=============================================================================
HRESULT DirectCore::InitDevice()
//
//
//
{
  HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( m_ref_hwnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    // Have no idea why, but on Windows 10 next line causes D3D11CreateDeviceAndSwapChain to fail
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_ref_hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    
    CComPtr<ID3D11Device> cp_device;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        hr = D3D11CreateDeviceAndSwapChain(
          nullptr, 
          driverTypes[driverTypeIndex], 
          nullptr, 
          createDeviceFlags, 
          featureLevels, 
          numFeatureLevels,
          D3D11_SDK_VERSION,
          &sd, 
          &m_cp_swap_chain, 
          &cp_device, 
          nullptr, 
          &m_cp_immediate_context 
        );

        if (SUCCEEDED(hr))
          break;
    }
    if (FAILED(hr))
      return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_cp_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if (FAILED(hr))
      return hr;

    hr = cp_device->CreateRenderTargetView( pBackBuffer, nullptr, &m_p_render_target_view );
    pBackBuffer->Release();
    if (FAILED(hr))
      return hr;
    
#ifdef DEPTH_STENCIL
    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    ID3D11Texture2D* p_texture2D;
    hr = cp_device->CreateTexture2D( &descDepth, nullptr, &p_texture2D );
    if (FAILED(hr))
      return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = cp_device->CreateDepthStencilView( p_texture2D, &descDSV, &m_depth_stencil_view );
    if (FAILED(hr))
        return hr;

    m_cp_immediate_context->OMSetRenderTargets( 1, &m_p_render_target_view, m_depth_stencil_view );
#else

    m_cp_immediate_context->OMSetRenderTargets( 1, &m_p_render_target_view, nullptr );
#endif
    

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_cp_immediate_context->RSSetViewports( 1, &vp );
        
  return S_OK;
}

//=============================================================================
void DirectCore::Clear()
{
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha

  m_cp_immediate_context->ClearRenderTargetView(
    m_p_render_target_view, 
    ClearColor 
  );

  m_cp_immediate_context->ClearDepthStencilView( 
    m_depth_stencil_view, 
    D3D11_CLEAR_DEPTH, 
    1.0f, 
    0
  );
}

//=============================================================================
void DirectCore::SwapChain()
{
  m_cp_swap_chain->Present( 0, 0 );
}

    
//=============================================================================
ID3D11DeviceContext* DirectCore::GetImmediateContext() const
{
  return m_cp_immediate_context;
}


//=============================================================================
