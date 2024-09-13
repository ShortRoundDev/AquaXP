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
        AQUAXP_API ~Graphics();

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
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}