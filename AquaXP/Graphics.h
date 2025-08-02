#pragma once
#include "Texture.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Shapes.h"

namespace AquaXP
{
#pragma warning( push )
#pragma warning( disable: 4251 )
    struct AQUAXP_API RenderTarget
    {
        Texture const* renderTargets;
        sz numRenderTargets;
        Texture const* depthBuffer;
        std::optional<bool> autosize;
    };
#pragma warning( pop )

    class Application;
    class Graphics
    {
    public:

        AQUAXP_API Graphics(
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
        );
        AQUAXP_API ~Graphics() = default;

        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DeviceContext> createDeferredContext() const;
        AQUAXP_API void renderDeferredContext(ID3D11DeviceContext* deferredContext, BOOL restoredImmediateContext = TRUE, BOOL restoreDeferredContext = FALSE) const;
        AQUAXP_API Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;
        AQUAXP_API DXGI_SAMPLE_DESC const& getMultiSamplingDesc() const;
        AQUAXP_API Texture const* getBackBuffer() const;

        AQUAXP_API void resetRenderTarget();
        AQUAXP_API void setRenderTarget(RenderTarget const& renderTarget);
        AQUAXP_API void setRenderTarget(
            Texture const* renderTarget,
            Texture const* depthBuffer
        );
        AQUAXP_API RenderTarget const& getRenderTarget() const;
        AQUAXP_API RenderTarget getRootRenderTarget() const;

        AQUAXP_API void setDepthStencilState(DepthStencilState const* depthStencilState, UINT stencilRef = 1);
        AQUAXP_API DepthStencilState const* getDepthStencilState() const;
        AQUAXP_API DepthStencilState const* getRootDepthStencilState() const;
        AQUAXP_API void resetDepthStencilState();

        AQUAXP_API void setRasterizerState(RasterizerState const* rasterizerState);
        AQUAXP_API RasterizerState const* getRasterizerState() const;
        AQUAXP_API RasterizerState const* getRootRasterizerState() const;
        AQUAXP_API void resetRasterizerState();

        AQUAXP_API void setViewPort(std::optional<D3D11_VIEWPORT const> viewPort = std::nullopt);
        AQUAXP_API D3D11_VIEWPORT const& getViewPort() const;
        AQUAXP_API D3D11_VIEWPORT const& getRootViewPort() const;

        AQUAXP_API void setDepthBuffer(Texture const* depthBuffer);
        AQUAXP_API Texture const* getDepthBuffer() const;
        AQUAXP_API Texture const* getRootDepthBuffer() const;

        AQUAXP_API Mesh<ScreenQuadVertex> const* getFullScreenQuad() const;

        AQUAXP_API void present() const;
        AQUAXP_API void restore();

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
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
        DepthStencilState const* m_depthStencilState;
        RasterizerState const* m_rasterizerState;

        std::unique_ptr<Texture const> m_rootDepthBuffer;
        std::unique_ptr<DepthStencilState const> m_rootDepthStencilState;
        std::unique_ptr<RasterizerState const> m_rootRasterizerState;
        std::unique_ptr<Mesh<ScreenQuadVertex> const> m_fullScreenQuad;
    };

    AQUAXP_API Result<std::unique_ptr<Graphics>> CreateGraphics(u16 width, u16 height, HWND hwnd, bool fullscreen, bool vsync);
}