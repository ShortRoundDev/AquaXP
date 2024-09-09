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

        AQUAXP_API ID3D11Device* getDevice() const;
        AQUAXP_API ID3D11DeviceContext* getContext() const;
        AQUAXP_API IDXGISwapChain* getSwapChain() const;

        AQUAXP_API void pushRenderTarget(RenderTarget const& renderTarget);
        AQUAXP_API void pushRenderTarget(Texture const& renderTarget);
        AQUAXP_API void pushRenderTarget(Texture const* renderTarget);
        AQUAXP_API void popRenderTarget();

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}