#include <assert.h>
#include <d3d11.h>
#include <stdio.h>

#include "cube-app.h"

namespace cube {

CubeApp::CubeApp(TCHAR const* const title, int width, int height)
    : physika::Application(title, width, height)
{
}

bool CubeApp::Initialize()
{
    //! Ensure base implementation is called at the beginning
    if (!physika::Application::Initialize()) {
        return false;
    }

    D3D_FEATURE_LEVEL featureLevel;

    HRESULT result = D3D11CreateDevice(
        nullptr,  //! select primary device
        D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
        nullptr,  // Select greatest feature level available
        0,        //! zero because previous param is nullptr
        D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext);

    if (FAILED(result)) {
        printf("Failed to initialize D3D11 Device");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.Width                 = this->mWidth;
    swapChainDesc.BufferDesc.Height                = this->mHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering =
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //! Enable MSAA?
    if (mEnableMSAA) {
        UINT msaaQuality;
        if (FAILED(mDevice->CheckMultisampleQualityLevels(
                DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality))) {
            printf("Failed to detect MSAA Quality");
            return false;  //! @todo Should this fall back to no MSAA if failed?
        }
        assert(msaaQuality > 0 && "MSAA Quality is not greater than 0");
        swapChainDesc.SampleDesc.Count   = 4;
        swapChainDesc.SampleDesc.Quality = msaaQuality - 1;
    } else {
        swapChainDesc.SampleDesc.Count   = 1;
        swapChainDesc.SampleDesc.Quality = 0;
    }

    swapChainDesc.BufferCount  = 1;
    swapChainDesc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = (HWND)this->WindowHandle();
    swapChainDesc.Windowed     = true;
    swapChainDesc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags        = 0;

    IDXGIDevice*  dxgiDevice  = 0;
    IDXGIAdapter* dxgiAdapter = 0;
    IDXGIFactory* dxgiFactory = 0;
    if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice),
                                       (void**)&dxgiDevice))) {
        printf("Failed to query device");
        assert(false && "Failed to query device");
    }
    if (FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
                                     (void**)&dxgiAdapter))) {
        printf("Failed to get dxgi adapter");
        assert(false && "Failed to get dxgi adapter");
    }
    if (FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
                                      (void**)&dxgiFactory))) {
        printf("Failed to get dxgi factory");
        assert(false && "Failed to get dxgi factory");
    }

    if (FAILED(dxgiFactory->CreateSwapChain(mDevice, &swapChainDesc,
                                            &mSwapchain))) {
        assert(false && "Failed to create swapchain");
    }

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();

    return true;
}

bool CubeApp::Shutdown()
{
    //! Ensure base implementation is called at the end
    return physika::Application::Shutdown();
}

void CubeApp::OnUpdate()
{
}

void CubeApp::OnResize(int /*width*/, int /*height*/)
{
}

void CubeApp::OnKeyUp(Keycode /*key*/)
{
}

void CubeApp::OnKeyDown(Keycode /*key*/)
{
}

void CubeApp::OnMouseUp(MouseButton /*button*/, int /*x*/, int /*y*/)
{
}

void CubeApp::OnMouseDown(MouseButton /*button*/, int /*x*/, int /*y*/)
{
}

void CubeApp::OnMouseMove(int /*x*/, int /*y*/)
{
}

void CubeApp::OnMouseWheel(int /*delta*/)
{
}

}  // namespace cube