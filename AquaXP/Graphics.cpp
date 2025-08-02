#include "pch.h"
#include "Graphics.h"
#include "Application.h"
#include "ContextHelpers.h"
#include "TextureBuilder.h"

#include <iostream>

using namespace AquaXP;
using namespace std;
using namespace Microsoft::WRL;

Graphics::Graphics(
    u16 width,
    u16 height,
    Microsoft::WRL::ComPtr<IDXGIFactory> factory,
    DXGI_MODE_DESC displayMode,
    Microsoft::WRL::ComPtr<ID3D11Device> device,
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain,
    DXGI_SAMPLE_DESC const& samplingDesc,
    Texture const& backBufferRenderTarget,
    Texture const& depthStencilBuffer,
    DepthStencilState depthStencilState,
    RasterizerState rasterizerState,
    Mesh<ScreenQuadVertex> const& fullScreenQuad
) : m_factory(factory),
    m_displayMode(displayMode),
    m_device(device),
    m_context(context),
    m_swapChain(swapChain),
    m_sampleDesc(samplingDesc),
    m_backBuffer(make_unique<Texture const>(backBufferRenderTarget)),
    m_rootDepthBuffer(make_unique<Texture const>(depthStencilBuffer)),
    m_rootDepthStencilState(make_unique<DepthStencilState const>(depthStencilState)),
    m_rootRasterizerState(make_unique<RasterizerState const>(rasterizerState)),
    m_fullScreenQuad(make_unique<Mesh<ScreenQuadVertex> const>(fullScreenQuad)),
    m_width(width),
    m_height(height),
    // To be initialized in the constructor body:
    m_viewPort(),
    m_rootViewPort(),
    m_msaaQualityLevels(),
    m_renderTarget(),
    m_depthBuffer(),
    m_depthStencilState(),
    m_rasterizerState()
{
    // Initialize to m_backBuffer and m_rootDepthBuffer
    resetRenderTarget();
}

Result<ComPtr<IDXGIFactory>> CreateDXGIFactory()
{
    ComPtr<IDXGIFactory> factory;
    auto res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());
    if (FAILED(res))
    {
        return HRToError(res);
    }

    return factory;
}

Result<DXGI_MODE_DESC> GetDisplayMode(u16 width, u16 height, IDXGIFactory* dxgiFactory)
{
    ComPtr<IDXGIAdapter> adapter = nullptr;
    auto adapterRes = dxgiFactory->EnumAdapters(0, &adapter);
    if (FAILED(adapterRes))
    {
        return HRToError(adapterRes);
    }

    ComPtr<IDXGIOutput> output = nullptr;
    auto outputRes = adapter->EnumOutputs(0, output.GetAddressOf());
    if (FAILED(outputRes))
    {
        return HRToError(outputRes);
    }

    UINT numModes = 0;
    auto numDisplayModeListRes = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    if (FAILED(numDisplayModeListRes))
    {
        return HRToError(numDisplayModeListRes);
    }

    auto displayModeList = std::make_unique<DXGI_MODE_DESC[]>(numModes);
    auto displayModeListRes = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.get());
    if (FAILED(displayModeListRes))
    {
        return HRToError(displayModeListRes);
    }

    optional<DXGI_MODE_DESC> displayMode;
    for (UINT i = 0; i < numModes; i++)
    {
        if (
            displayModeList[i].Width == width &&
            displayModeList[i].Height == height)
        {
            displayMode = displayModeList[i];
            break;
        }
    }
    if (!displayMode.has_value())
    {
        return ErrorCode::NoDisplayModeFound;
    }

    return displayMode.value();
}

Result<tuple<ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>>> CreateDeviceAndContext()
{
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    
    auto res = D3D11CreateDevice(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        (BYTE)D3D11_CREATE_DEVICE_DEBUG,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        device.GetAddressOf(),
        NULL,
        context.GetAddressOf()
    );
    if (FAILED(res))
    {
        return HRToError(res);
    }

    return make_tuple(device, context);
}

Result<tuple<ComPtr<IDXGISwapChain>, DXGI_SAMPLE_DESC>> CreateSwapChain(
    ID3D11Device* device,
    IDXGIFactory* factory,
    DXGI_MODE_DESC displayMode,
    HWND hwnd,
    u16 width,
    u16 height,
    bool fullscreen
)
{
    UINT maxQuality;
    constexpr u32 COUNT = 4;

    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, COUNT, &maxQuality);

    DXGI_SWAP_CHAIN_DESC swapchainDesc{};

    swapchainDesc.BufferCount = 1;
    swapchainDesc.BufferDesc.Width = static_cast<u32>(width);
    swapchainDesc.BufferDesc.Height = static_cast<u32>(height);
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = displayMode.RefreshRate.Numerator;// or 0 if !vsync
    swapchainDesc.BufferDesc.RefreshRate.Denominator = displayMode.RefreshRate.Denominator; // or 1 if !vsync
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = hwnd;
    swapchainDesc.SampleDesc.Count = COUNT;
    swapchainDesc.SampleDesc.Quality = maxQuality - 1;
    swapchainDesc.Windowed = !fullscreen;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapchainDesc.Flags = 0;

    ComPtr<IDXGISwapChain> swapchain = nullptr;

    auto res = factory->CreateSwapChain(
        device,
        &swapchainDesc,
        swapchain.GetAddressOf()
    );

    if (FAILED(res))
    {
        return HRToError(res);
    }

    ComPtr<IDXGIOutput> output;
    auto outputRes = swapchain->GetContainingOutput(output.GetAddressOf());
    if (FAILED(outputRes))
    {
        return HRToError(outputRes);
    }

    return make_tuple(swapchain, swapchainDesc.SampleDesc);
}

Result<Texture> CreateBackBufferRenderTarget(IDXGISwapChain* swapChain, ID3D11Device* device)
{
    ComPtr<ID3D11Texture2D> backBuffer;
    auto backBufferRes = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    if (FAILED(backBufferRes))
    {
        return HRToError(backBufferRes);
    }

    auto backBufferTextureRes = TextureBuilder()
        .withTexture(backBuffer)
        .withRTV()
        .build(device);
    if (!isOk(backBufferTextureRes))
    {
        return error(backBufferTextureRes);
    }
    return get(backBufferTextureRes);
}

Result<tuple<Texture, DepthStencilState>> CreateDepthStencilBuffer(
    ID3D11Device* device,
    u16 width,
    u16 height,
    DXGI_SAMPLE_DESC samplingDesc
)
{
    auto depthRes = CreateDepthTarget(
        device,
        static_cast<u32>(width),
        static_cast<u32>(height),
        samplingDesc
    );
    if (!isOk(depthRes))
    {
        return error(depthRes);
    }

    auto depthStencilStateRes = CreateDepthStencilState(device);
    if (!isOk(depthStencilStateRes))
    {
        return error(depthStencilStateRes);
    }

    return make_tuple(get(depthRes), get(depthStencilStateRes));
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

    m_context->OMSetRenderTargets(
        static_cast<u32>(renderTarget.numRenderTargets),
        renderTarget.numRenderTargets > 0 ? s_renderTargetViews : nullptr,
        m_depthBuffer == nullptr ? nullptr : m_depthBuffer->getDepthStencilView().Get()
    );
    
    // Default to autosizing but allow the user to change it
    if (!renderTarget.autosize.value_or(true))
    {
        return;
    }

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

void Graphics::setRenderTarget(
    Texture const* renderTarget,
    Texture const* depthBuffer
)
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

Mesh<ScreenQuadVertex> const* Graphics::getFullScreenQuad() const
{
    return m_fullScreenQuad.get();
}

RenderTarget const& Graphics::getRenderTarget() const
{
    return m_renderTarget;
}

RenderTarget Graphics::getRootRenderTarget() const {
    return {
        .renderTargets = m_backBuffer.get(),
        .numRenderTargets = 1,
        .depthBuffer = m_depthBuffer
    };
}

void Graphics::setDepthStencilState(DepthStencilState const* depthStencilState, UINT stencilRef)
{
    /* Depth Stencil state must ALWAYS be set */
    m_depthStencilState = depthStencilState == nullptr
        ? m_rootDepthStencilState.get()
        : depthStencilState;

    m_context->OMSetDepthStencilState(m_depthStencilState->Get(), stencilRef);
}

DepthStencilState const* Graphics::getDepthStencilState() const
{
    return m_depthStencilState;
}

DepthStencilState const* Graphics::getRootDepthStencilState() const
{
    return m_rootDepthStencilState.get();
}

void Graphics::resetDepthStencilState()
{
    setDepthStencilState(m_rootDepthStencilState.get());
}

void Graphics::setRasterizerState(RasterizerState const* rasterizerState)
{
    m_rasterizerState = rasterizerState;
    m_context->RSSetState(m_rasterizerState->Get());
}

RasterizerState const* Graphics::getRasterizerState() const
{
    return m_rasterizerState;
}

RasterizerState const* Graphics::getRootRasterizerState() const
{
    return m_rootRasterizerState.get();
}

void Graphics::resetRasterizerState()
{
    setRasterizerState(m_rootRasterizerState.get());
}

void Graphics::present() const
{
    m_swapChain->Present(1, 0);
}

Result<unique_ptr<Graphics>> AquaXP::CreateGraphics(u16 width, u16 height, HWND hwnd, bool fullscreen, bool vsync)
{
    auto factoryRes = CreateDXGIFactory();
    if (!isOk(factoryRes))
    {
        return error(factoryRes);
    }

    auto dxgiFactory = get(factoryRes);

    auto displayModeRes = GetDisplayMode(width, height, dxgiFactory.Get());
    if (!isOk(displayModeRes))
    {
        return error(displayModeRes);
    }

    auto deviceContextRes = CreateDeviceAndContext();
    if (!isOk(deviceContextRes))
    {
        return error(deviceContextRes);
    }

    auto [device, context] = get(deviceContextRes);
    auto swapChainRes = CreateSwapChain(device.Get(), dxgiFactory.Get(), get(displayModeRes), hwnd, width, height, fullscreen);
    if (!isOk(swapChainRes))
    {
        return error(swapChainRes);
    }

    auto [swapChain, samplingDesc] = get(swapChainRes);
    auto backBufferRes = CreateBackBufferRenderTarget(swapChain.Get(), device.Get());
    if (!isOk(backBufferRes))
    {
        return error(backBufferRes);
    }

    auto depthStencilBufferRes = CreateDepthStencilBuffer(device.Get(), width, height, samplingDesc);
    if (!isOk(depthStencilBufferRes))
    {
        return error(depthStencilBufferRes);
    }

    auto [depthStencilBuffer, depthStencilState] = get(depthStencilBufferRes);

    auto rasterizerStateRes = CreateRasterizerState(device.Get());
    if (!isOk(rasterizerStateRes))
    {
        return error(rasterizerStateRes);
    }

    auto fullScreenQuadRes = CreateFullScreenQuad(device.Get());
    if (!isOk(fullScreenQuadRes))
    {
        return error(fullScreenQuadRes);
    }

    return make_unique<Graphics>(
        width,
        height,
        dxgiFactory,
        get(displayModeRes),
        device,
        context,
        swapChain,
        samplingDesc,
        get(backBufferRes),
        depthStencilBuffer,
        depthStencilState,
        get(rasterizerStateRes),
        get(fullScreenQuadRes)
    );
}

void Graphics::restore()
{
    setRenderTarget(m_renderTarget);
    setDepthStencilState(m_depthStencilState);
    setRasterizerState(m_rasterizerState);
}