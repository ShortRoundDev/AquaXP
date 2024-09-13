#include "pch.h"
#include "Texture.h"
#include "Graphics.h"
#include "StringHelpers.h"

using namespace AquaXP;
using namespace std;

class Texture::impl
{
public:
    impl(ID3D11Device* device, ID3D11DeviceContext* context, std::string const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE) :
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

        if (FAILED(CreateWICTextureFromFileEx(
            device,
            context,
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
        };

        D3D11_TEXTURE2D_DESC desc;
        m_texture2D->GetDesc(&desc);

        m_width = static_cast<f32>(desc.Width);
        m_height = static_cast<f32>(desc.Height);

        m_status = initializeResources(device, flags);
    }

    impl(ID3D11Device* device, ID3D11DeviceContext* context, std::wstring const& path, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE) :
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
        if (FAILED(CreateWICTextureFromFileEx(
            device,
            context,
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
        }

        D3D11_TEXTURE2D_DESC desc;
        m_texture2D->GetDesc(&desc);

        m_width = static_cast<f32>(desc.Width);
        m_height = static_cast<f32>(desc.Height);

        m_status = initializeResources(device, flags);
    }

    impl(ID3D11Device* device, ID3D11DeviceContext* context, u8 const* data, sz size, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE) :
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
        if (FAILED(CreateWICTextureFromMemoryEx(
            device,
            context,
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

        m_status = initializeResources(device, flags);
    }

    impl(ID3D11Device* device, f32 width, f32 height, D3D11_BIND_FLAG flags = D3D11_BIND_SHADER_RESOURCE) :
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

        if (FAILED(device->CreateTexture2D(&desc, nullptr, &m_texture2D)))
        {
            m_status = false;
            return;
        }

        if (flags & D3D11_BIND_SHADER_RESOURCE)
        {
            if (FAILED(device->CreateShaderResourceView(m_texture2D.Get(), nullptr, &m_shaderResourceView)))
            {
                m_status = false;
                return;
            }
        }

        m_status = initializeResources(device, flags);
    }

    impl(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Texture2D> raw, D3D11_BIND_FLAG flags) :
        m_resource(raw),
        m_texture2D(raw),
        m_depthStencilTexture(),
        m_shaderResourceView(),
        m_depthStencilView(),
        m_renderTargetView(),
        m_flags(flags),
        m_width(),
        m_height(),
        m_status(false)
    {
        D3D11_TEXTURE2D_DESC desc;
        m_texture2D->GetDesc(&desc);

        m_width = static_cast<f32>(desc.Width);
        m_height = static_cast<f32>(desc.Height);

        if (flags & D3D11_BIND_SHADER_RESOURCE)
        {
            if (FAILED(device->CreateShaderResourceView(m_texture2D.Get(), nullptr, m_shaderResourceView.GetAddressOf())))
            {
                m_status = false;
                return;
            }
        }

        m_status = initializeResources(device, flags);
    }

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

    bool initializeResources(ID3D11Device* device, D3D11_BIND_FLAG flags)
    {
        D3D11_TEXTURE2D_DESC desc;
        m_texture2D->GetDesc(&desc);

        if (flags & D3D11_BIND_RENDER_TARGET)
        {
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = desc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;

            if (FAILED(device->CreateRenderTargetView(m_texture2D.Get(), &rtvDesc, &m_renderTargetView)))
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

            if (FAILED(device->CreateTexture2D(&depthStencilDesc, nullptr, &m_depthStencilTexture)))
            {
                return false;
            }

            D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
            dsvDesc.Format = depthStencilDesc.Format;
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Texture2D.MipSlice = 0;

            if (FAILED(device->CreateDepthStencilView(m_depthStencilTexture.Get(), &dsvDesc, &m_depthStencilView)))
            {
                return false;
            }
        }

        return true;
    }
};


Texture::Texture(Graphics* graphics, std::string const& path, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), graphics->getContext().Get(), path, flags) { }

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* context, std::string const& path, D3D11_BIND_FLAG flags) :
    m_pimpl(std::make_unique<Texture::impl>(device, context, path, flags)) { }

Texture::Texture(Graphics* graphics, std::wstring const& path, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), graphics->getContext().Get(), path, flags) { }

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* context, std::wstring const& path, D3D11_BIND_FLAG flags) :
    m_pimpl(std::make_unique<Texture::impl>(device, context, path, flags)) { }

Texture::Texture(Graphics* graphics, u8 const* data, sz size, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), graphics->getContext().Get(), data, size, flags) { }

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* context, u8 const* data, sz size, D3D11_BIND_FLAG flags) :
    m_pimpl(std::make_unique<Texture::impl>(device, context, data, size, flags)) { }

Texture::Texture(Graphics* graphics, f32 width, f32 height, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), width, height, flags) { }

Texture::Texture(ID3D11Device* device, f32 width, f32 height, D3D11_BIND_FLAG flags) :
    m_pimpl(std::make_unique<Texture::impl>(device, width, height, flags)) { }

Texture::Texture(Graphics* graphics, Microsoft::WRL::ComPtr<ID3D11Texture2D> raw, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), raw, flags) { }

Texture::Texture(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Texture2D> raw, D3D11_BIND_FLAG flags) :
    m_pimpl(std::make_unique<Texture::impl>(device, raw, flags)) { }


Texture::~Texture() = default;

void Texture::use(Graphics const* graphics, u32 slot) const
{
    if (m_pimpl->m_shaderResourceView)
    {
        graphics->getContext()->PSSetShaderResources(slot, 1, m_pimpl->m_shaderResourceView.GetAddressOf());
    }
}

Microsoft::WRL::ComPtr<ID3D11Resource> Texture::getResource() const
{
    return m_pimpl->m_resource;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture::getTexture2D() const
{
    return m_pimpl->m_texture2D;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::getShaderResourceView() const
{
    return m_pimpl->m_shaderResourceView;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Texture::getDepthStencilView() const
{
    return m_pimpl->m_depthStencilView;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Texture::getRenderTargetView() const
{
    return m_pimpl->m_renderTargetView;
}

f32 Texture::getWidth() const
{
    return m_pimpl->m_width;
}

f32 Texture::getHeight() const
{
    return m_pimpl->m_height;
}

D3D11_BIND_FLAG Texture::getFlags() const
{
    return m_pimpl->m_flags;
}

bool Texture::getStatus() const
{
    return m_pimpl->m_status;
}

void Texture::clear(Graphics const* graphics, f32 const clearColor[4])
{
    if (m_pimpl->m_renderTargetView)
    {
        graphics
            ->getContext()
            ->ClearRenderTargetView(m_pimpl->m_renderTargetView.Get(), clearColor);
    }
}

void Texture::clearDepth(Graphics const* graphics)
{
    if (m_pimpl->m_depthStencilView)
    {
        graphics
            ->getContext()
            ->ClearDepthStencilView(
                m_pimpl->m_depthStencilView.Get(),
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                1.0f,
                0
            );
    }
}
