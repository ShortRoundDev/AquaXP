#pragma once
#include "Texture.h"

namespace AquaXP
{
    struct AQUAXP_API RenderTarget
    {
        Texture const* renderTargets;
        sz numRenderTargets;
        Texture const* depthBuffer;
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
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DeviceContext> createDeferredContext() const;
        AQUAXP_API void renderDeferredContext(ID3D11DeviceContext* deferredContext, BOOL restoredImmediateContext = TRUE, BOOL restoreDeferredContext = FALSE) const;
        AQUAXP_API Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;
        AQUAXP_API DXGI_SAMPLE_DESC const& getMultiSamplingDesc() const;
        AQUAXP_API Texture const* getBackBuffer() const;
        AQUAXP_API std::unique_ptr<Texture const> moveBackBuffer();

        AQUAXP_API void resetRenderTarget();
        AQUAXP_API void setRenderTarget(RenderTarget const& renderTarget);
        AQUAXP_API void setRenderTarget(Texture const* renderTarget, Texture const* depthBuffer);
        AQUAXP_API RenderTarget const& getRenderTarget() const;

        AQUAXP_API void setViewPort(std::optional<D3D11_VIEWPORT const> viewPort = std::nullopt);
        AQUAXP_API D3D11_VIEWPORT const& getViewPort() const;
        AQUAXP_API D3D11_VIEWPORT const& getRootViewPort() const;

        AQUAXP_API void setDepthBuffer(Texture const* depthBuffer);
        AQUAXP_API Texture const* getDepthBuffer() const;
        AQUAXP_API Texture const* getRootDepthBuffer() const;
        AQUAXP_API std::unique_ptr<Texture const> moveRootDepthBuffer();

        AQUAXP_API void present() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer;
        Microsoft::WRL::ComPtr<IDXGIFactory> m_factory;

        D3D11_VIEWPORT m_viewPort;
        D3D11_VIEWPORT m_rootViewPort;

        DXGI_SAMPLE_DESC m_sampleDesc;

        UINT m_msaaQualityLevels;

        u16 m_width;
        u16 m_height;

        DXGI_MODE_DESC m_displayMode;

        std::unique_ptr<Texture const> m_backBuffer;
        RenderTarget m_renderTarget;
        Texture const* m_depthBuffer;
        std::unique_ptr<Texture const> m_rootDepthBuffer;

        using Initializer = bool(Graphics::*)(Application* application);
        bool initWaterfall(
            Application* application,
            std::initializer_list<Initializer> initializers
        );

        bool initInfrastructure(u16 width, u16 height);
        bool initSwapchain(HWND hwnd, bool fullscreen);
        bool initRenderTarget(u16 width, u16 height);
        bool initDepthStencilBuffer(u16 width, u16 height);
        bool initRasterizer();

    };
}