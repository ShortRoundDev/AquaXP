#pragma once

namespace AquaXP
{
    constexpr f32 CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    class Graphics;
    class Texture
    {
    public:
        AQUAXP_API Texture(Graphics* graphics, std::string const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, ID3D11DeviceContext* context, std::string const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics* graphics, std::wstring const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, ID3D11DeviceContext* context, std::wstring const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics* graphics, u8 const* data, sz size, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, ID3D11DeviceContext* context, u8 const* data, sz size, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics* graphics, f32 width, f32 height, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, f32 width, f32 height, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API Texture(Graphics* graphics, Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        AQUAXP_API Texture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        AQUAXP_API ~Texture();

        AQUAXP_API void use(Graphics const* graphics, u32 slot = TEX_ALBEDO) const;
        AQUAXP_API void use(ID3D11DeviceContext* device, u32 slot = TEX_ALBEDO) const;

        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11Resource> getResource() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture2D() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11DepthStencilState> getDepthStencilState() const;
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;

        AQUAXP_API f32 getWidth() const;
        AQUAXP_API f32 getHeight() const;

        AQUAXP_API D3D11_BIND_FLAG getFlags() const;
        AQUAXP_API bool getStatus() const;

        AQUAXP_API void clear(Graphics const* graphics, f32 const clearColor[4] = CLEAR_COLOR) const;
        AQUAXP_API void clearDepth(Graphics const* graphics) const;

    private:
        bool m_status;

        Microsoft::WRL::ComPtr<ID3D11Resource> m_resource;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilTexture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

        D3D11_BIND_FLAG m_flags;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

        f32 m_width;
        f32 m_height;

        bool initializeResources(ID3D11Device* device, D3D11_BIND_FLAG flags);
    };
}
