#include "pch.h"
#include "Graphics.h"
#include "Application.h"
#include "ContextHelpers.h"

#include <iostream>

using namespace AquaXP;
using namespace std;
using namespace Microsoft::WRL;

Graphics::Graphics(
    u16 width,
    u16 height,
    HWND hwnd,
    bool fullscreen
) : m_context(),
    m_device(),
    m_swapChain(),
    m_backBuffer(),
    m_width(width),
    m_height(height)
{
    if (!initInfrastructure(width, height)) {
        std::cout << "Failed to init infrastructure" << std::endl;
    }
    if (!initSwapchain(hwnd, fullscreen)) {
        std::cout << "Failed to init swapchain" << std::endl;
    }
    if (!initRenderTarget(width, height)) {
        std::cout << "Failed to init rtv" << std::endl;
    }
    if (!initDepthStencilBuffer(width, height)) {
        std::cout << "Failed to init depth buffer" << std::endl;
    }
    setRenderTarget(m_backBuffer.get(), m_rootDepthBuffer.get());
    if (!initRasterizer()) {
        std::cout << "Failed to init rasterizer!" << std::endl;
    }
}


bool Graphics::initWaterfall(
    Application* application,
    std::initializer_list<Initializer> initializers
)
{
    for (auto initializer : initializers)
    {
        if (!(this->*initializer)(application))
        {
            return false;
        }
    }

    return true;
}


bool Graphics::initInfrastructure(u16 width, u16 height)
{
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_factory.GetAddressOf())))
    {
        return false;
    }

    IDXGIAdapter* adapter = nullptr;
    if (FAILED(m_factory->EnumAdapters(0, &adapter)))
    {
        return false;
    }

    ComPtr<IDXGIOutput> output = nullptr;
    if (FAILED(adapter->EnumOutputs(0, &output)))
    {
        return false;
    }

    UINT numModes = 0;
    if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
    {
        return false;
    }

    auto displayModeList = std::make_unique<DXGI_MODE_DESC[]>(numModes);
    if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.get())))
    {
        return false;
    }

    for (UINT i = 0; i < numModes; i++)
    {
        if (
            displayModeList[i].Width == width &&
            displayModeList[i].Height == height)
        {
            m_displayMode = displayModeList[i];
            break;
        }
    }
    return true;
}

bool Graphics::initSwapchain(HWND hwnd, bool fullscreen)
{

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    if (FAILED(D3D11CreateDevice(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        (BYTE)D3D11_CREATE_DEVICE_DEBUG,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        m_device.GetAddressOf(),
        NULL,
        m_context.GetAddressOf()
    )))
    {
        return false;
    }

    UINT maxQuality;
    const u32 COUNT = 4;
    m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, COUNT, &maxQuality);

    DXGI_SWAP_CHAIN_DESC swapchainDesc = { 0 };
    swapchainDesc.BufferCount = 1;
    swapchainDesc.BufferDesc.Width = static_cast<u32>(m_width);
    swapchainDesc.BufferDesc.Height = static_cast<u32>(m_height);
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 0; // or 0 if !vsync
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 1; // or 1 if !vsync
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = hwnd;
    swapchainDesc.SampleDesc.Count = COUNT;
    swapchainDesc.SampleDesc.Quality = maxQuality - 1;
    swapchainDesc.Windowed = !fullscreen;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapchainDesc.Flags = 0;

    m_sampleDesc = swapchainDesc.SampleDesc;

    HRESULT res;
    if (FAILED(
        res = m_factory->CreateSwapChain(
            m_device.Get(),
            &swapchainDesc,
            m_swapChain.GetAddressOf()
        )
    ))
    {
        return false;
    }

    ComPtr<IDXGIOutput> output;
    if (FAILED(m_swapChain->GetContainingOutput(output.GetAddressOf())))
    {
        return false;
    }

    return true;
}

bool Graphics::initRenderTarget(u16 width, u16 height)
{
    ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf())))
    {
        return false;
    }

    m_backBuffer = std::make_unique<Texture>(
        *this,
        backBuffer,
        static_cast<D3D11_BIND_FLAG>(D3D11_BIND_RENDER_TARGET | D3D11_BIND_DEPTH_STENCIL)
    );

    return true;
}

bool Graphics::initDepthStencilBuffer(u16 width, u16 height)
{
    m_rootDepthBuffer = make_unique<Texture>(
        *this,
        static_cast<f32>(width), static_cast<f32>(height),
        D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL
    );

    m_depthBuffer = m_rootDepthBuffer.get();

    return true;
}

bool Graphics::initRasterizer()
{
    D3D11_RASTERIZER_DESC rasterDesc = { };
    ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterDesc.AntialiasedLineEnable = FALSE;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.MultisampleEnable = TRUE;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;
    rasterDesc.FrontCounterClockwise = false;

    if (FAILED(m_device->CreateRasterizerState(&rasterDesc, m_rasterizer.GetAddressOf())))
    {
        return false;
    }

    m_context->RSSetState(m_rasterizer.Get());
    return true;
}

void Graphics::resetRenderTarget()
{
    setRenderTarget(m_backBuffer.get(), m_rootDepthBuffer.get());
}

void Graphics::setRenderTarget(RenderTarget const& renderTarget)
{
    static thread_local ID3D11RenderTargetView* s_renderTargetViews[8];

    m_renderTarget = renderTarget;
    ZeroMemory(s_renderTargetViews, sizeof(s_renderTargetViews));

    for (i32 i = 0; i < renderTarget.numRenderTargets; i++)
    {
        s_renderTargetViews[i] = renderTarget.renderTargets[i].getRenderTargetView().Get();
    }

    m_depthBuffer = renderTarget.depthBuffer;

    m_context->OMSetDepthStencilState(m_depthBuffer->getDepthStencilState().Get(), 1);
    m_context->OMSetRenderTargets(
        static_cast<u32>(renderTarget.numRenderTargets),
        renderTarget.numRenderTargets > 0 ? s_renderTargetViews : nullptr,
        m_depthBuffer == nullptr ? nullptr : m_depthBuffer->getDepthStencilView().Get()
    );

    f32 width = renderTarget.numRenderTargets > 0
        ? renderTarget.renderTargets[0].getWidth()
        : (renderTarget.depthBuffer != nullptr
            ? renderTarget.depthBuffer->getWidth()
            : 0),
    height = renderTarget.numRenderTargets > 0
        ? renderTarget.renderTargets[0].getHeight()
        : (renderTarget.depthBuffer != nullptr
            ? renderTarget.depthBuffer->getHeight()
            : 0
            );

    D3D11_VIEWPORT viewport = {
        .Width = width,
        .Height = height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f
    };

    setViewPort(viewport);
}

void Graphics::setViewPort(std::optional<D3D11_VIEWPORT const> viewPort)
{
    m_viewPort = viewPort.value_or(m_rootViewPort);
    m_context->RSSetViewports(1, &m_viewPort);
}

D3D11_VIEWPORT const& Graphics::getViewPort() const
{
    return m_viewPort;
}

D3D11_VIEWPORT const& Graphics::getRootViewPort() const
{
    return m_rootViewPort;
}

void Graphics::setDepthBuffer(Texture const* texture)
{
    m_depthBuffer = texture;
}

ComPtr<ID3D11Device> Graphics::getDevice() const
{
    return m_device;
}

ComPtr<ID3D11DeviceContext> Graphics::getContext() const
{
    return m_context;
}

ComPtr<ID3D11DeviceContext> Graphics::createDeferredContext() const
{
    ComPtr<ID3D11DeviceContext> deferredContext;
    if (FAILED(m_device->CreateDeferredContext(0u, deferredContext.GetAddressOf())))
    {
        return nullptr;
    }
    return deferredContext;
}

AQUAXP_API void Graphics::renderDeferredContext(ID3D11DeviceContext* deferredContext, BOOL restoredImmediateContext, BOOL restoreDeferredContext) const
{
    auto commandList = GetCommandList(deferredContext, restoreDeferredContext);
    m_context->ExecuteCommandList(commandList.Get(), restoredImmediateContext);
}

ComPtr<IDXGISwapChain> Graphics::getSwapChain() const
{
    return m_swapChain;
}

DXGI_SAMPLE_DESC const& Graphics::getMultiSamplingDesc() const
{
    return m_sampleDesc;
}

Texture const* Graphics::getBackBuffer() const
{
    return m_backBuffer.get();
}

std::unique_ptr<Texture const> Graphics::moveBackBuffer()
{
    return std::move(m_backBuffer);
}

void Graphics::setRenderTarget(Texture const* renderTarget, Texture const* depthBuffer)
{
    RenderTarget rt = {
        renderTarget,
        static_cast<sz>(renderTarget == nullptr ? 0 : 1),
        depthBuffer
    };
    setRenderTarget(rt);
}

Texture const* Graphics::getDepthBuffer() const
{
    return m_depthBuffer;
}

Texture const* Graphics::getRootDepthBuffer() const
{
    return m_rootDepthBuffer.get();
}

unique_ptr<Texture const> Graphics::moveRootDepthBuffer()
{
    return std::move(m_rootDepthBuffer);
}

RenderTarget const& Graphics::getRenderTarget() const
{
    return m_renderTarget;
}

void Graphics::present() const
{
    m_swapChain->Present(1, 0);
}
