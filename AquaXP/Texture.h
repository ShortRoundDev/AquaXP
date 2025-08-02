#pragma once

#include "Result.h"

namespace AquaXP
{
    constexpr f32 CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

#pragma warning( push )
#pragma warning( disable: 4251 )
    struct SRVOptions
    {
        std::optional<DXGI_FORMAT> format;
        std::optional<D3D11_SRV_DIMENSION> viewDimension;
    };
    
    struct RTVOptions
    {
        std::optional<DXGI_FORMAT> format;
        std::optional<D3D11_RTV_DIMENSION> viewDimension;
    };

    struct DSVOptions
    {
        DXGI_FORMAT format;
        std::optional<D3D11_DSV_DIMENSION> viewDimension;
        std::optional<UINT> flags;
    };

    struct TextureSampleOptions
    {
        std::optional<UINT> count;
        std::optional<UINT> quality;
    };

    struct TextureOptions
    {
        u32 width;
        u32 height;
        DXGI_FORMAT format;
        DXGI_SAMPLE_DESC sampleDesc;
        std::optional<UINT> bindFlags;
        std::optional<UINT> mipLevels;
        std::optional<UINT> arraySize;
        std::optional<D3D11_USAGE> usage;
        std::optional<UINT> cpuAccessFlags;
        std::optional<UINT> miscFlags;
    };
#pragma warning( pop )

    class Graphics;
    class Texture
    {
    public:
        AQUAXP_API Texture(
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture,
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView,
            Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView,
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView
        );

        AQUAXP_API ~Texture();

        AQUAXP_API void use(Graphics const& graphics, u32 slot = TEX_ALBEDO) const;
        AQUAXP_API void use(ID3D11DeviceContext* device, u32 slot = TEX_ALBEDO) const;

        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture2D() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;

        AQUAXP_API f32 getWidth() const;
        AQUAXP_API f32 getHeight() const;

        AQUAXP_API void clear(Graphics const& graphics, f32 const clearColor[4] = CLEAR_COLOR) const;
        AQUAXP_API void clearDepth(Graphics const& graphics, f32 value = 1.0f) const;
        AQUAXP_API void clearStencil(Graphics const& graphics, u32 value = 0) const;
        AQUAXP_API void clearDepthStencil(Graphics const& graphics, f32 depth = 1.0f, u32 stencil = 0) const;
        

    private:
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

        D3D11_TEXTURE2D_DESC m_desc;
    };

    AQUAXP_API Result<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> CreateShaderResourceView(
        ID3D11Device* device,
        ID3D11Texture2D* texture,
        std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> desc = std::nullopt
    );

    AQUAXP_API Result<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> CreateDepthStencilView(
        ID3D11Device* device,
        ID3D11Texture2D* texture,
        D3D11_DEPTH_STENCIL_VIEW_DESC const& desc
    );

    AQUAXP_API Result<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> CreateRenderTargetView(
        ID3D11Device* device,
        ID3D11Texture2D* texture,
        D3D11_RENDER_TARGET_VIEW_DESC const& desc
    );

    AQUAXP_API Result<Microsoft::WRL::ComPtr<ID3D11Texture2D>> CreateTexture2D(
        ID3D11Device* device,
        D3D11_TEXTURE2D_DESC const& desc
    );

    AQUAXP_API Result<Texture> CreateDepthTarget(
        ID3D11Device* device,
        u32 width,
        u32 height,
        DXGI_SAMPLE_DESC sampleDesc,
        bool isSrv = true
    );

    AQUAXP_API Result<Texture> CreateRenderTarget(
        ID3D11Device* device,
        u32 width,
        u32 height,
        DXGI_SAMPLE_DESC sampleDesc,
        bool isSrv = true,
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM
    );

    AQUAXP_API Result<Texture> LoadTextureFromFile(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        std::string const& path
    );
}
