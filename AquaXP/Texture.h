#pragma once

#include <string>

#include "d3d11.h"
#include <wrl.h>
#include <wrl/client.h>

#include "Types.h
#include "Slots.h"

#include "Graphics.h"

namespace AquaXP
{
    constexpr f32 CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    class Graphics;
    class Texture
    {
    public:
        Texture(Graphics const* graphics, std::string const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        Texture(Graphics const* graphics, std::wstring const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        Texture(Graphics const* graphics, u8 const* data, sz size, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);
        Texture(Graphics const* graphics, f32 width, f32 height, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE);

        void use(Graphics const* graphics, u32 slot = TEX_ALBEDO) const;

        Microsoft::WRL::ComPtr<ID3D11Resource> getResource() const;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture2D() const;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDepthStencilView() const;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRenderTargetView() const;

        D3D11_BIND_FLAG getFlags() const;
        bool getStatus() const;

        void clear(Graphics const* graphics, f32 const clearColor[4] = CLEAR_COLOR);
        void clearDepth(Graphics const* graphics);

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

        bool initializeResources(Graphics const* graphics, D3D11_BIND_FLAG flags);
    };
}
