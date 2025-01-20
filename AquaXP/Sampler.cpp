#include "pch.h"
#include "Sampler.h"

using namespace std;
using namespace DirectX;
using namespace AquaXP;

constexpr u16 FILTER_MASK = 0b11111;

static inline D3D11_TEXTURE_ADDRESS_MODE getAddressMode(TextureAddressMode mode)
{
    switch (mode)
    {
    case WRAP_U:
    {
        return D3D11_TEXTURE_ADDRESS_WRAP;
    }
    case MIRROR_U:
    {
        return D3D11_TEXTURE_ADDRESS_MIRROR;
    }
    case BORDER_U:
    {
        return D3D11_TEXTURE_ADDRESS_BORDER;
    }
    case CLAMP_U:
    {
        return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
    case MIRROR_ONCE_U:
    {
        return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
    }
    default:
    {
        return D3D11_TEXTURE_ADDRESS_WRAP;
    }
    }
}

Sampler::Sampler(
    ID3D11Device* device,
    D3D11_FILTER filterType,
    TextureAddressMode textureAddressMode,
    XMFLOAT4 border
)
{
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = filterType;
    D3D11_TEXTURE_ADDRESS_MODE
        u = getAddressMode(static_cast<TextureAddressMode>(
            static_cast<int>(textureAddressMode) & FILTER_MASK
        )),
        v = getAddressMode(static_cast<TextureAddressMode>(
            (static_cast<int>(textureAddressMode) >> 5) & FILTER_MASK
        )),
        w = getAddressMode(static_cast<TextureAddressMode>(
            (static_cast<int>(textureAddressMode) >> 10) & FILTER_MASK
        ));

    samplerDesc.AddressU = u;
    samplerDesc.AddressV = v;
    samplerDesc.AddressW = w;

    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = border.x;
    samplerDesc.BorderColor[1] = border.y;
    samplerDesc.BorderColor[2] = border.z;
    samplerDesc.BorderColor[3] = border.w;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0.0f;

    auto result = device->CreateSamplerState(
        &samplerDesc,
        m_samplerState.GetAddressOf()
    );
    if (FAILED(result))
    {
        // TODO: Handle errors here
        return;
    }
}

void Sampler::use(ID3D11DeviceContext* context, u32 slot) const
{
    context->PSSetSamplers(slot, 1, m_samplerState.GetAddressOf());
}
