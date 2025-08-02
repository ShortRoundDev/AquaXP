#include "pch.h"
#include "Texture.h"
#include "Graphics.h"
#include "StringHelpers.h"

using namespace AquaXP;
using namespace std;
using namespace Microsoft::WRL;

Texture::Texture(
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView,
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView,
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView
) : m_texture2D(texture),
    m_shaderResourceView(shaderResourceView),
    m_depthStencilView(depthStencilView),
    m_renderTargetView(renderTargetView)
{
    m_texture2D.Get()->GetDesc(&m_desc);
}

Texture::~Texture() = default;

void Texture::use(ID3D11DeviceContext* context, u32 slot) const
{
    if (m_shaderResourceView)
    {
        context->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
    }
}
void Texture::use(Graphics const& graphics, u32 slot) const
{
    use(graphics.getContext().Get(), slot);
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

f32 Texture::getWidth() const
{
    return static_cast<f32>(m_desc.Width);
}

f32 Texture::getHeight() const
{
    return static_cast<f32>(m_desc.Height);
}

void Texture::clear(Graphics const& graphics, f32 const clearColor[4]) const
{
    if (m_renderTargetView)
    {
        graphics
            .getContext()
            ->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    }
}

void Texture::clearDepth(Graphics const& graphics, f32 value) const
{
    if (m_depthStencilView)
    {
        graphics
            .getContext()
            ->ClearDepthStencilView(
                m_depthStencilView.Get(),
                D3D11_CLEAR_DEPTH,
                value,
                0
            );
    }
}

void Texture::clearStencil(Graphics const& graphics, u32 value) const
{
    if (m_depthStencilView)
    {
        graphics
            .getContext()
            ->ClearDepthStencilView(
                m_depthStencilView.Get(),
                D3D11_CLEAR_STENCIL,
                0.0f,
                value
            );
    }
}

void Texture::clearDepthStencil(Graphics const& graphics, f32 depth, u32 stencil) const {
    if (m_depthStencilView)
    {
        graphics
            .getContext()
            ->ClearDepthStencilView(
                m_depthStencilView.Get(),
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                depth,
                stencil
            );
    }
}

Result<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> AquaXP::CreateShaderResourceView(
    ID3D11Device* device,
    ID3D11Texture2D* texture,
    std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> desc
)
{
    ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
    D3D11_SHADER_RESOURCE_VIEW_DESC* realDesc = desc.has_value() ? &(desc.value()) : nullptr;
    if (FAILED(device->CreateShaderResourceView(texture, realDesc, &shaderResourceView)))
    {
        return ErrorCode::SRVCreationFailed;
    }
    return shaderResourceView;
}

Result<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> AquaXP::CreateDepthStencilView(
    ID3D11Device* device,
    ID3D11Texture2D* texture,
    D3D11_DEPTH_STENCIL_VIEW_DESC const& desc
)
{
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
    if (FAILED(device->CreateDepthStencilView(texture, &desc, &depthStencilView)))
    {
        return ErrorCode::DSVCreationFailed;
    }
    return depthStencilView;
}

Result<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> AquaXP::CreateRenderTargetView(
    ID3D11Device* device,
    ID3D11Texture2D* texture,
    D3D11_RENDER_TARGET_VIEW_DESC const& desc
)
{
    ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
    if (FAILED(device->CreateRenderTargetView(texture, &desc, &renderTargetView)))
    {
        return ErrorCode::RTVCreationFailed;
    }
    return renderTargetView;
}

Result<Microsoft::WRL::ComPtr<ID3D11Texture2D>> AquaXP::CreateTexture2D(
    ID3D11Device* device,
    D3D11_TEXTURE2D_DESC const& desc
)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;

    if (FAILED(device->CreateTexture2D(&desc, nullptr, &texture)))
    {
        return ErrorCode::DepthBufferCreationFailed;
    }
    return texture;
}

Result<Texture> AquaXP::CreateDepthTarget(
    ID3D11Device* device,
    u32 width,
    u32 height,
    DXGI_SAMPLE_DESC sampleDesc,
    bool isSrv
)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;

    D3D11_TEXTURE2D_DESC desc = { };
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = isSrv ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc = sampleDesc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (isSrv ? D3D11_BIND_SHADER_RESOURCE : 0);
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    auto textureResult = CreateTexture2D(device, desc);
    if (auto error = errorOpt(textureResult))
    {
        return error.value();
    }
    auto texture = get(textureResult);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    dsvDesc.Texture2D.MipSlice = 0;

    auto dsvResult = CreateDepthStencilView(device, texture.Get(), dsvDesc);
    if (auto error = errorOpt(dsvResult))
    {
        return error.value();
    }
    auto depthStencilView = get(dsvResult);

    if (isSrv)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

        auto srvResult = CreateShaderResourceView(device, texture.Get(), srvDesc);
        if (auto error = errorOpt(srvResult))
        {
            return error.value();
        }
        shaderResourceView = get(srvResult);
    }

    return Texture(
        texture,
        shaderResourceView,
        depthStencilView,
        nullptr
    );
}

Result<Texture> AquaXP::CreateRenderTarget(
    ID3D11Device* device,
    u32 width,
    u32 height,
    DXGI_SAMPLE_DESC sampleDesc,
    bool isSrv,
    DXGI_FORMAT format
)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;

    D3D11_TEXTURE2D_DESC desc = { };
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc = sampleDesc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | (isSrv ? D3D11_BIND_SHADER_RESOURCE : 0);
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    auto textureResult = CreateTexture2D(device, desc);
    if (auto error = errorOpt(textureResult))
    {
        return error.value();
    }

    auto texture = get(textureResult);

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = { };
    ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtvDesc.Format = format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

    auto rtvResult = CreateRenderTargetView(device, texture.Get(), rtvDesc);
    if (auto error = errorOpt(rtvResult))
    {
        return error.value();
    }

    auto rtv = get(rtvResult);

    if (isSrv)
    {
        auto srvResult = CreateShaderResourceView(device, texture.Get());
        if (auto error = errorOpt(srvResult))
        {
            return error.value();
        }
        shaderResourceView = get(srvResult);
    }

    return Texture(
        texture,
        shaderResourceView,
        nullptr,
        rtv
    );
}

Result<Texture> AquaXP::LoadTextureFromFile(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    std::string const& path
)
{
    std::wstring wPath;
    if (!mbStrToWideChar(path, wPath))
    {
        return ErrorCode::FileNotFound;
    }
    ComPtr<ID3D11Resource> buffer;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    if (FAILED(CreateWICTextureFromFileEx(
        device,
        context,
        wPath.c_str(),
        0L,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE,
        0,
        D3D11_RESOURCE_MISC_GENERATE_MIPS,
        DirectX::WIC_LOADER_FLAGS::WIC_LOADER_FORCE_RGBA32,
        buffer.GetAddressOf(),
        shaderResourceView.GetAddressOf()
    )))
    {
        return ErrorCode::WICError;
    }

    ComPtr<ID3D11Texture2D> texture;
    buffer.As(&texture);

    return Texture(
        texture,
        shaderResourceView,
        nullptr,
        nullptr
    );
}