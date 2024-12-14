#pragma once
#include "Texture.h"

namespace AquaXP
{
    struct AQUAXP_API RenderTarget
    {
        Texture const* renderTargets;
        sz numRenderTargets;
    };

    class Application;
    class Graphics
    {
    public:
        AQUAXP_API Graphics(
            u16 width,
            u16 height,
            HWND hwnd,
            bool fullscreen
        );
        AQUAXP_API ~Graphics() = default;

        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext() const;
        AQUAXP_API Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;
        AQUAXP_API Texture const* getBackBuffer() const;

        AQUAXP_API void setRenderTarget(RenderTarget const& renderTarget);
        AQUAXP_API void setRenderTarget(Texture const* renderTarget);
        AQUAXP_API RenderTarget const& getRenderTarget() const;

        AQUAXP_API void setDepthBuffer(Texture const* depthBuffer);
        AQUAXP_API Texture const* getDepthBuffer() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        DXGI_MODE_DESC m_displayMode;

        std::unique_ptr<Texture const> m_backBuffer;
        RenderTarget m_renderTarget;
        Texture const* m_depthBuffer;

        using Initializer = bool(Graphics::*)(Application* application);
        bool initWaterfall(
            Application* application,
            std::initializer_list<Initializer> initializers
        );

        bool initInfrastructure(u16 width, u16 height);
        bool initSwapchain(HWND hwnd, bool fullscreen);
        bool initRenderTarget(u16 width, u16 height);

    };
}