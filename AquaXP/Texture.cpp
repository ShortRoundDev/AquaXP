#include "pch.h"
#include "Texture.h"
#include "Graphics.h"
#include "StringHelpers.h"

using namespace AquaXP;

Texture::Texture(Graphics const* graphics, std::string const& path, D3D11_BIND_FLAG flags) :
    m_resource(),
    m_texture2D(),
    m_depthStencilTexture(),
    m_shaderResourceView(),
    m_depthStencilView(),
    m_renderTargetView(),
    m_flags(flags),
    m_width(),
    m_height(),
    m_status(false)
{
    std::wstring wpath;
    if (!mbStrToWideChar(path, wpath))
    {
        m_status = false;
    }

    if (FAILED(DirectX::CreateWICTextureFromFileEx(
        graphics->getDevice().Get(),
        graphics->getContext().Get(),
        wpath.c_str(),
        0L,
        D3D11_USAGE_DEFAULT,
        flags,
        0,
        D3D11_RESOURCE_MISC_GENERATE_MIPS,
        DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
        m_resource.GetAddressOf(),
        m_shaderResourceView.GetAddressOf()
    )))
    {
        m_status = false;
        return;
    }

    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);

    m_width = static_cast<f32>(desc.Width);
    m_height = static_cast<f32>(desc.Height);

    m_status = initializeResources(graphics, flags);
}

Texture::Texture(Graphics const* graphics, std::wstring const& path, D3D11_BIND_FLAG flags) :
    m_resource(),
    m_texture2D(),
    m_depthStencilTexture(),
    m_shaderResourceView(),
    m_depthStencilView(),
    m_renderTargetView(),
    m_flags(flags),
    m_width(),
    m_height(),
    m_status(false)
{
    if (FAILED(DirectX::CreateWICTextureFromFileEx(
        graphics->getDevice().Get(),
        graphics->getContext().Get(),
        path.c_str(),
        0L,
        D3D11_USAGE_DEFAULT,
        flags,
        0,
        D3D11_RESOURCE_MISC_GENERATE_MIPS,
        DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
        m_resource.GetAddressOf(),
        m_shaderResourceView.GetAddressOf()
    )))
    {
        m_status = false;
        return;
    };

    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);

    m_width = static_cast<f32>(desc.Width);
    m_height = static_cast<f32>(desc.Height);

    m_status = initializeResources(graphics, flags);
}

Texture::Texture(Graphics const* graphics, u8 const* data, sz size, D3D11_BIND_FLAG flags) :
    m_resource(),
    m_texture2D(),
    m_depthStencilTexture(),
    m_shaderResourceView(),
    m_depthStencilView(),
    m_renderTargetView(),
    m_flags(),
    m_width(),
    m_height(),
    m_status(false)
{
    if (FAILED(DirectX::CreateWICTextureFromMemoryEx(
        graphics->getDevice().Get(),
        graphics->getContext().Get(),
        data,
        size,
        0L,
        D3D11_USAGE_DEFAULT,
        flags,
        0,
        0,
        DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
        m_resource.GetAddressOf(),
        m_shaderResourceView.GetAddressOf()
    )))
    {
        m_status = false;
        return;
    }

    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);
    m_width = static_cast<f32>(desc.Width);
    m_height = static_cast<f32>(desc.Height);

    m_status = initializeResources(graphics, flags);
}

Texture::Texture(Graphics const* graphics, f32 width, f32 height, D3D11_BIND_FLAG flags) :
    m_resource(),
    m_texture2D(),
    m_depthStencilTexture(),
    m_shaderResourceView(),
    m_depthStencilView(),
    m_renderTargetView(),
    m_flags(flags),
    m_width(width),
    m_height(height),
    m_status(false)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    desc.Width = static_cast<u32>(width);
    desc.Height = static_cast<u32>(height);
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = flags;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    if (FAILED(graphics->getDevice()->CreateTexture2D(&desc, nullptr, &m_texture2D)))
    {
        m_status = false;
        return;
    }

    if (flags & D3D11_BIND_SHADER_RESOURCE)
    {
        if (FAILED(graphics->getDevice()->CreateShaderResourceView(m_texture2D.Get(), nullptr, &m_shaderResourceView)))
        {
            m_status = false;
            return;
        }
    }

    m_status = initializeResources(graphics, flags);
}

bool Texture::initializeResources(Graphics const* graphics, D3D11_BIND_FLAG flags)
{
    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);

    if (flags & D3D11_BIND_RENDER_TARGET)
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = desc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        if (FAILED(graphics->getDevice()->CreateRenderTargetView(m_texture2D.Get(), &rtvDesc, &m_renderTargetView)))
        {
            return false;
        }
    }

    if (flags & D3D11_BIND_DEPTH_STENCIL)
    {
        D3D11_TEXTURE2D_DESC depthStencilDesc = {};
        depthStencilDesc.Width = desc.Width;
        depthStencilDesc.Height = desc.Height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        if (FAILED(graphics->getDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &m_depthStencilTexture)))
        {
            return false;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = depthStencilDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;

        if (FAILED(graphics->getDevice()->CreateDepthStencilView(m_depthStencilTexture.Get(), &dsvDesc, &m_depthStencilView)))
        {
            return false;
        }
    }

    return true;
}

void Texture::use(Graphics const* graphics, u32 slot) const
{
    if (m_shaderResourceView)
    {
        graphics->getContext()->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
    }
}

Microsoft::WRL::ComPtr<ID3D11Resource> Texture::getResource() const
{
    return m_resource;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture::getTexture2D() const
{
    return m_texture2D;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::getShaderResourceView() const
{
    return m_shaderResourceView;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Texture::getDepthStencilView() const
{
    return m_depthStencilView;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Texture::getRenderTargetView() const
{
    return m_renderTargetView;
}

D3D11_BIND_FLAG Texture::getFlags() const
{
    return m_flags;
}

bool Texture::getStatus() const
{
    return m_status;
}

void Texture::clear(Graphics const* graphics, f32 const clearColor[4])
{
    if (m_renderTargetView)
    {
        graphics
            ->getContext()
            ->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    }
}

void Texture::clearDepth(Graphics const* graphics)
{
    if (m_depthStencilView)
    {
        graphics
            ->getContext()
            ->ClearDepthStencilView(
                m_depthStencilView.Get(),
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f,
                0
            );
    }
}

