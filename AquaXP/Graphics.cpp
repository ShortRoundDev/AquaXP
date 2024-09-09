#include "pch.h"
#include "Graphics.h"

#include "Application.h"

using namespace AquaXP;
using namespace std;

class Graphics::impl
{
public:
    impl(
        u16 width,
        u16 height,
        HWND hwnd,
        bool fullscreen
    ) : m_context(),
        m_device(),
        m_swapChain(),
        m_backBuffer()
    {
        initInfrastructure(width, height);
        initSwapchain(hwnd, fullscreen);
        initRenderTarget(width, height);
    }

    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    DXGI_MODE_DESC m_displayMode;

    std::unique_ptr<Texture const> m_backBuffer;
    std::stack<RenderTarget> m_renderTargets;

    using Initializer = bool(Graphics::impl::*)(Application* application);
    bool initWaterfall(
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


    bool initInfrastructure(u16 width, u16 height)
    {
        Microsoft::WRL::ComPtr<IDXGIFactory> factory = nullptr;
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf())))
        {
            return false;
        }

        IDXGIAdapter* adapter = nullptr;
        if (FAILED(factory->EnumAdapters(0, &adapter)))
        {
            return false;
        }

        Microsoft::WRL::ComPtr<IDXGIOutput> output = nullptr;
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

    bool initSwapchain(HWND hwnd, bool fullscreen)
    {
        DXGI_SWAP_CHAIN_DESC swapchainDesc = { 0 };
        swapchainDesc.BufferCount = 1;
        swapchainDesc.BufferDesc = m_displayMode;
        swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.OutputWindow = hwnd;
        swapchainDesc.SampleDesc.Count = 1;
        swapchainDesc.Windowed = !fullscreen;
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
        if (FAILED(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0, // TODO: Replace this with a configurable flag
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &swapchainDesc,
            m_swapChain.GetAddressOf(),
            m_device.GetAddressOf(),
            nullptr,
            m_context.GetAddressOf()
        )))
        {
            return false;
        }

        Microsoft::WRL::ComPtr<IDXGIOutput> output;
        if (FAILED(m_swapChain->GetContainingOutput(output.GetAddressOf())))
        {
            return false;
        }

        return true;
    }

    bool initRenderTarget(u16 width, u16 height)
    {
        m_backBuffer = std::make_unique<Texture>(
            m_device.Get(),
            m_context.Get(),
            static_cast<f32>(width),
            static_cast<f32>(height),
            static_cast<D3D11_BIND_FLAG>(D3D11_BIND_RENDER_TARGET)
        );
        return true;
    }

    void setRenderTarget(RenderTarget const& renderTarget)
    {
        static thread_local ID3D11RenderTargetView* s_renderTargetViews[8];
        ZeroMemory(s_renderTargetViews, sizeof(s_renderTargetViews));

        if (renderTarget.renderTargets == nullptr || renderTarget.numRenderTargets == 0)
        {
            return;
        }

        for (i32 i = 0; i < renderTarget.numRenderTargets; i++)
        {
            s_renderTargetViews[i] = renderTarget.renderTargets[i].getRenderTargetView();
        }

        m_context->OMSetRenderTargets(
            static_cast<u32>(renderTarget.numRenderTargets),
            s_renderTargetViews,
            renderTarget.depthStencil->getDepthStencilView()
        );
    }
};

Graphics::Graphics(
    u16 width,
    u16 height,
    HWND hwnd,
    bool fullscreen
)
    : m_pimpl(std::make_unique<Graphics::impl>(width, height, hwnd, fullscreen))
{
}

Graphics::~Graphics() = default;

ID3D11Device* Graphics::getDevice() const
{
    return m_pimpl->m_device.Get();
}

ID3D11DeviceContext* Graphics::getContext() const
{
    return m_pimpl->m_context.Get();
}

IDXGISwapChain* Graphics::getSwapChain() const
{
    return m_pimpl->m_swapChain.Get();
}

void Graphics::pushRenderTarget(RenderTarget const& renderTarget)
{
    m_pimpl->m_renderTargets.push(renderTarget);
    m_pimpl->setRenderTarget(renderTarget);
}

void Graphics::pushRenderTarget(Texture const& renderTarget)
{
    RenderTarget rt = {
        &renderTarget,
        1,
        (renderTarget.getFlags() & D3D11_BIND_DEPTH_STENCIL)
            ? &renderTarget
            : nullptr
    };
    pushRenderTarget(rt);
}

void Graphics::pushRenderTarget(Texture const* renderTarget)
{
    RenderTarget rt = {
        renderTarget,
        1,
        (renderTarget->getFlags() & D3D11_BIND_DEPTH_STENCIL)
            ? renderTarget
            : nullptr
    };
    pushRenderTarget(rt);
}

void Graphics::popRenderTarget()
{

    if (!m_pimpl->m_renderTargets.empty())
    {
        m_pimpl->m_renderTargets.pop();
    }

    if (m_pimpl->m_renderTargets.empty())
    {
        RenderTarget rt = {
            m_pimpl->m_backBuffer.get(),
            1,
            m_pimpl->m_backBuffer.get()
        };
        m_pimpl->setRenderTarget(rt);
    }
    else
    {
        m_pimpl->setRenderTarget(m_pimpl->m_renderTargets.top());
    }
}