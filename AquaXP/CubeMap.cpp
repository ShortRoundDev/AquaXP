#include "pch.h"
#include "CubeMap.h"

using namespace AquaXP;
using namespace std;
using namespace DirectX;

CubeMap::CubeMap(ID3D11Device* device, wstring const& path) :
    CubeMap(device, path.c_str()) { }

CubeMap::CubeMap(ID3D11Device* device, WCHAR const* path) :
    m_shaderResourceView(nullptr),
    m_shadowMapFacesDsv({ 0 }),
    m_depthMapTexture2D(nullptr)
{
    HRESULT res = CreateDDSTextureFromFile(
        device,
        path,
        nullptr, m_shaderResourceView.GetAddressOf()
    );
    if (FAILED(res))
    {
        //TODO: Handle errors
        return;
    }
}

CubeMap::CubeMap(ID3D11Device* device, u16 width, u16 height) :
    m_shaderResourceView(nullptr),
    m_shadowMapFacesDsv({ 0 }),
    m_depthMapTexture2D(nullptr)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = static_cast<UINT>(width);
    desc.Height = static_cast<UINT>(height);
    desc.MipLevels = 1;
    desc.ArraySize = 6;
    desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    device->CreateTexture2D(&desc, 0, m_depthMapTexture2D.GetAddressOf());
    if (m_depthMapTexture2D.Get() == nullptr)
    {
        //TODO: Handle errors
        return;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsv;
    dsv.Flags = 0;
    dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    dsv.Texture2DArray.MipSlice = 0;
    dsv.Texture2DArray.ArraySize = 1;
    dsv.Texture2DArray.FirstArraySlice = 0;

    for (int i = 0; i < 6; i++)
    {
        dsv.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);
        device->CreateDepthStencilView(m_depthMapTexture2D.Get(), &dsv, m_shadowMapFacesDsv[i].GetAddressOf());
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srv;
    srv.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srv.TextureCube.MipLevels = desc.MipLevels;
    srv.TextureCube.MostDetailedMip = 0;
    auto result = device->CreateShaderResourceView(m_depthMapTexture2D.Get(), &srv, m_shaderResourceView.GetAddressOf());
}

void CubeMap::use(ID3D11DeviceContext* context, u32 const slot)
{
    context->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}

std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6> const& CubeMap::getDepthStencilViews() const
{
    return m_shadowMapFacesDsv;
}

