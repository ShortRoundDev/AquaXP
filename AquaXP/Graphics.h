#pragma once
#include "Texture.h"

namespace AquaXP
{
    struct AQUAXP_API RenderTarget
    {
        Texture const* renderTargets;
        sz numRenderTargets;
        Texture const* depthStencil;
    };

    class Application;
    class AQUAXP_API Graphics
    {
    public:
        Graphics(Application* application);

        Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext() const;
        Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;

        void pushRenderTarget(RenderTarget const& renderTarget);
        void pushRenderTarget(Texture const& renderTarget);
        void pushRenderTarget(Texture const* renderTarget);
        void popRenderTarget();

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        DXGI_MODE_DESC m_displayMode;

        std::unique_ptr<Texture const> m_backBuffer;
        std::stack<RenderTarget> m_renderTargets;

        using Initializer = bool(Graphics::*)(Application* application);
        bool initWaterfall(Application* application, std::initializer_list<Initializer> initializers);

        bool initInfrastructure(Application* application);
        bool initSwapchain(Application* application);
        bool initRenderTarget(Application* application);

        void setRenderTarget(RenderTarget const& renderTarget);
    };
}