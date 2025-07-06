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
        std::optional<DXGI_FORMAT> format;
        std::optional<D3D11_DSV_DIMENSION> viewDimension;
        UINT flags;
    };

    struct TextureOptions
    {
        u32 width;
        u32 height;
        DXGI_FORMAT format;
        UINT bindFlags;
        std::optional<UINT> mipLevels;
        std::optional<UINT> arraySize;
        std::optional<DXGI_SAMPLE_DESC> sampleDesc;
        std::optional<D3D11_USAGE> usage;
        std::optional<SRVOptions> srv;
        std::optional<RTVOptions> rtv;
        std::optional<DSVOptions> dsv;
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
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView,
            D3D11_BIND_FLAG flags
        );

        AQUAXP_API Texture(Graphics const& graphics, std::string const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(Graphics const& graphics, char const* path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, ID3D11DeviceContext* context, std::string const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics const& graphics, std::wstring const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(Graphics const& graphics, wchar_t const* path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, ID3D11DeviceContext* context, std::wstring const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics const& graphics, u8 const* data, sz size, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, ID3D11DeviceContext* context, u8 const* data, sz size, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics const& graphics, f32 width, f32 height, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE, std::optional<DXGI_FORMAT> format = std::nullopt);
        AQUAXP_API Texture(Graphics const& graphics, f32 width, f32 height, DXGI_SAMPLE_DESC const& sampleDesc, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE, std::optional<DXGI_FORMAT> format = std::nullopt);
        AQUAXP_API Texture(ID3D11Device* device, f32 width, f32 height, DXGI_SAMPLE_DESC const& sampleDesc = { .Count = 1, .Quality = 0 }, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE, std::optional<DXGI_FORMAT> format = std::nullopt);

        AQUAXP_API Texture(Graphics const& graphics, Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API ~Texture();

        AQUAXP_API void use(Graphics const& graphics, u32 slot = TEX_ALBEDO) const;
        AQUAXP_API void use(ID3D11DeviceContext* device, u32 slot = TEX_ALBEDO) const;

        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture2D() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;

        AQUAXP_API f32 getWidth() const;
        AQUAXP_API f32 getHeight() const;

        AQUAXP_API D3D11_BIND_FLAG getFlags() const;
        AQUAXP_API bool getStatus() const;

        AQUAXP_API void clear(Graphics const& graphics, f32 const clearColor[4] = CLEAR_COLOR) const;
        AQUAXP_API void clearDepth(Graphics const& graphics, f32 value = 1.0f) const;
        AQUAXP_API void clearStencil(Graphics const& graphics, u32 value = 0) const;
        AQUAXP_API void clearDepthStencil(Graphics const& graphics, f32 depth = 1.0f, u32 stencil = 0) const;
        

    private:
        bool m_status;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

        D3D11_BIND_FLAG m_flags;

        f32 m_width;
        f32 m_height;

        bool initializeResources(ID3D11Device* device, D3D11_BIND_FLAG flags);
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
}
