#include "pch.h"
#include "Texture.h"
#include "Graphics.h"
#include "StringHelpers.h"

using namespace AquaXP;
using namespace std;

Texture::Texture(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    std::string const& path,
    D3D11_BIND_FLAG flags
) :
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
    Microsoft::WRL::ComPtr<ID3D11Resource> buffer;
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
        buffer.GetAddressOf(),
        m_shaderResourceView.GetAddressOf()
    )))
    {
        m_status = false;
        return;
    };
    buffer.As(&m_texture2D);

    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);

    m_width = static_cast<f32>(desc.Width);
    m_height = static_cast<f32>(desc.Height);

    m_status = initializeResources(device, flags);
}

Texture::Texture(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    std::wstring const& path,
    D3D11_BIND_FLAG flags
) :
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
    Microsoft::WRL::ComPtr<ID3D11Resource> buffer;
    HRESULT res = CreateWICTextureFromFileEx(
        device,
        context,
        path.c_str(),
        0L,
        D3D11_USAGE_DEFAULT,
        flags,
        0,
        D3D11_RESOURCE_MISC_GENERATE_MIPS,
        DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
        buffer.GetAddressOf(),
        m_shaderResourceView.GetAddressOf()
    );
    if (FAILED(res ))
    {
        m_status = false;
        return;
    }
    buffer.As(&m_texture2D);

    res = m_texture2D->QueryInterface(__uuidof(ID3D11Texture2D), (void**)m_texture2D.GetAddressOf());
    if (FAILED(res))
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

Texture::Texture(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    u8 const* data,
    sz size,
    D3D11_BIND_FLAG flags
) :
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
    Microsoft::WRL::ComPtr<ID3D11Resource> buffer;
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
        buffer.GetAddressOf(),
        m_shaderResourceView.GetAddressOf()
    )))
    {
        m_status = false;
        return;
    }
    buffer.As(&m_texture2D);

    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);
    m_width = static_cast<f32>(desc.Width);
    m_height = static_cast<f32>(desc.Height);

    m_status = initializeResources(device, flags);
}

Texture::Texture(
    ID3D11Device* device,
    f32 width,
    f32 height,
    DXGI_SAMPLE_DESC const& sampleDesc,
    D3D11_BIND_FLAG flags
) :
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
    desc.Format = (flags & D3D11_BIND_DEPTH_STENCIL) ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc = sampleDesc;
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

Texture::Texture(
    ID3D11Device* device,
    Microsoft::WRL::ComPtr<ID3D11Texture2D> raw,
    D3D11_BIND_FLAG flags
) :
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

bool Texture::initializeResources(ID3D11Device* device, D3D11_BIND_FLAG flags)
{
    D3D11_TEXTURE2D_DESC desc;
    m_texture2D->GetDesc(&desc);

    if (flags & D3D11_BIND_RENDER_TARGET)
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = { };
        ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        rtvDesc.Format = desc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

        if (FAILED(device->CreateRenderTargetView(m_texture2D.Get(), &rtvDesc, &m_renderTargetView)))
        {
            return false;
        }
    }

    if (flags & D3D11_BIND_DEPTH_STENCIL)
    {
        D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
        depthStencilTextureDesc.Width = desc.Width;
        depthStencilTextureDesc.Height = desc.Height;
        depthStencilTextureDesc.MipLevels = 1;
        depthStencilTextureDesc.ArraySize = 1;
        depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilTextureDesc.SampleDesc = desc.SampleDesc;
        depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        if (FAILED(device->CreateTexture2D(&depthStencilTextureDesc, nullptr, &m_depthStencilTexture)))
        {
            return false;
        }

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };
        ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        depthStencilDesc.StencilEnable = true;
        depthStencilDesc.StencilReadMask = 0xff;
        depthStencilDesc.StencilWriteMask = 0xff;
        depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf())))
        {
            return false;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = depthStencilTextureDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        dsvDesc.Texture2D.MipSlice = 0;

        if (FAILED(device->CreateDepthStencilView(m_depthStencilTexture.Get(), &dsvDesc, &m_depthStencilView)))
        {
            return false;
        }
    }

    return true;
}


Texture::Texture(Graphics* graphics, std::string const& path, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), graphics->getContext().Get(), path, flags) { }

Texture::Texture(Graphics* graphics, char const* path, D3D11_BIND_FLAG flags) :
    Texture(graphics, string(path), flags) { }

Texture::Texture(Graphics* graphics, std::wstring const& path, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), graphics->getContext().Get(), path, flags) { }

Texture::Texture(Graphics* graphics, wchar_t const* path, D3D11_BIND_FLAG flags) :
    Texture(graphics, wstring(path), flags) { }

Texture::Texture(Graphics* graphics, f32 width, f32 height, DXGI_SAMPLE_DESC const& sampleDesc, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), width, height, sampleDesc, flags) { }

Texture::Texture(Graphics* graphics, u8 const* data, sz size, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), graphics->getContext().Get(), data, size, flags) { }

Texture::Texture(Graphics* graphics, f32 width, f32 height, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), width, height, graphics->getMultiSamplingDesc(), flags) { }

Texture::Texture(Graphics* graphics, Microsoft::WRL::ComPtr<ID3D11Texture2D> raw, D3D11_BIND_FLAG flags) :
    Texture(graphics->getDevice().Get(), raw, flags) { }

Texture::~Texture() = default;

void Texture::use(ID3D11DeviceContext* context, u32 slot) const
{
    if (m_shaderResourceView)
    {
        context->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
    }
}
void Texture::use(Graphics const* graphics, u32 slot) const
{
    use(graphics->getContext().Get(), slot);
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

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> Texture::getDepthStencilState() const
{
    return m_depthStencilState;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Texture::getRenderTargetView() const
{
    return m_renderTargetView;
}

f32 Texture::getWidth() const
{
    return m_width;
}

f32 Texture::getHeight() const
{
    return m_height;
}

D3D11_BIND_FLAG Texture::getFlags() const
{
    return m_flags;
}

bool Texture::getStatus() const
{
    return m_status;
}

void Texture::clear(Graphics const* graphics, f32 const clearColor[4]) const
{
    if (m_renderTargetView)
    {
        graphics
            ->getContext()
            ->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    }
}

void Texture::clearDepth(Graphics const* graphics) const
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
