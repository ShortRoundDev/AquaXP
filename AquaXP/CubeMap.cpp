#include "pch.h"
#include "CubeMap.h"

using namespace AquaXP;
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

CubeMap::CubeMap(
    ComPtr<ID3D11Texture2D> texture2D,
    ComPtr<ID3D11ShaderResourceView> shaderResourceView,
    optional<array<ComPtr<ID3D11RenderTargetView>, 6>> renderTargetViews,
    optional<array<ComPtr<ID3D11DepthStencilView>, 6>> depthStencilViews
) : m_texture2D(texture2D),
    m_shaderResourceView(shaderResourceView),
    m_renderTargetViews(renderTargetViews),
    m_depthStencilViews(depthStencilViews)
{}

void CubeMap::use(ID3D11DeviceContext* context, u32 const slot)
{
    context->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}

optional<reference_wrapper<const array<ComPtr<ID3D11DepthStencilView>, 6>>> CubeMap::getDepthStencilViews() const
{
    if (m_depthStencilViews.has_value())
    {
        return cref(m_depthStencilViews.value());
    }
    return m_depthStencilViews;
}

optional<reference_wrapper<const array<ComPtr<ID3D11RenderTargetView>, 6>>> CubeMap::getRenderTargetViews() const
{
    if (m_renderTargetViews.has_value())
    {
        return cref(m_renderTargetViews.value());
    }
    return m_renderTargetViews;
}

void foo(CubeMap const& cmap)
{
}