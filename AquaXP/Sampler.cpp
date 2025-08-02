#include "pch.h"
#include "Sampler.h"

using namespace std;
using namespace DirectX;
using namespace AquaXP;

constexpr u16 FILTER_MASK = 0b11111;

static D3D11_TEXTURE_ADDRESS_MODE getAddressMode(TextureAddressMode mode)
{
    switch (mode)
    {
    case WrapU:
    {
        return D3D11_TEXTURE_ADDRESS_WRAP;
    }
    case MirrorU:
    {
        return D3D11_TEXTURE_ADDRESS_MIRROR;
    }
    case BorderU:
    {
        return D3D11_TEXTURE_ADDRESS_BORDER;
    }
    case ClampU:
    {
        return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
    case MirrorOnceU:
    {
        return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
    }
    default:
    {
        return D3D11_TEXTURE_ADDRESS_WRAP;
    }
    }
}

Result<Sampler> AquaXP::CreateSampler(ID3D11Device* device, SamplerOptions const& options)
{
    D3D11_SAMPLER_DESC samplerDesc;
    auto textureAddressMode = options.textureAddressMode.value_or(WrapAll);
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

    samplerDesc.Filter = options.filter.value_or(D3D11_FILTER_ANISOTROPIC);
    samplerDesc.AddressU = u;
    samplerDesc.AddressV = v;
    samplerDesc.AddressW = w;
    samplerDesc.MipLODBias = options.mipLODBias.value_or(0.0f);
    samplerDesc.MaxAnisotropy = options.maxAnisotropy.value_or(16);
    samplerDesc.ComparisonFunc = options.comparisonFunc.value_or(D3D11_COMPARISON_ALWAYS);
    auto borderColor = options.borderColor.value_or(XMFLOAT4(0, 0, 0, 0));
    samplerDesc.BorderColor[0] = borderColor.x;
    samplerDesc.BorderColor[1] = borderColor.y;
    samplerDesc.BorderColor[2] = borderColor.z;
    samplerDesc.BorderColor[3] = borderColor.w;
    samplerDesc.MinLOD = options.minLOD.value_or(0);
    samplerDesc.MaxLOD = options.maxLOD.value_or(D3D11_FLOAT32_MAX);
    samplerDesc.MipLODBias = options.mipLODBias.value_or(0.0f);

    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    auto res = device->CreateSamplerState(
        &samplerDesc,
        samplerState.GetAddressOf()
    );
    if (FAILED(res))
    {
        return HRToError(res);
    }
    return Sampler(samplerState);
}

AQUAXP_API void AquaXP::UseSampler(ID3D11DeviceContext* context, Sampler const& sampler, u32 slot, Bind bind)
{
    if (HasFlag(bind, Bind::VS))
    {
        context->VSSetSamplers(slot, 1, sampler.GetAddressOf());
    }
    if (HasFlag(bind, Bind::PS))
    {
        context->PSSetSamplers(slot, 1, sampler.GetAddressOf());
    }
    if (HasFlag(bind, Bind::GS))
    {
        context->GSSetSamplers(slot, 1, sampler.GetAddressOf());
    }
    if (HasFlag(bind, Bind::HS))
    {
        context->HSSetSamplers(slot, 1, sampler.GetAddressOf());
    }
    if (HasFlag(bind, Bind::CS))
    {
        context->CSSetSamplers(slot, 1, sampler.GetAddressOf());
    }
    if (HasFlag(bind, Bind::DS))
    {
        context->DSSetSamplers(slot, 1, sampler.GetAddressOf());
    }
}