#pragma once

namespace AquaXP
{
    enum TextureAddressMode : u16
    {
        WrapU        = 0b0000000000000001,
        MirrorU      = 0b0000000000000010,
        BorderU      = 0b0000000000000100,
        ClampU       = 0b0000000000001000,
        MirrorOnceU  = 0b0000000000010000,

        WrapV        = 0b0000000000100000,
        MirrorV      = 0b0000000001000000,
        BorderV      = 0b0000000010000000,
        ClampV       = 0b0000000100000000,
        MirrorOnceV  = 0b0000001000000000,

        WrapW        = 0b0000010000000000,
        MirrorW      = 0b0000100000000000,
        BorderW      = 0b0001000000000000,
        ClampW       = 0b0010000000000000,
        MirrorOnceW  = 0b0100000000000000,

        WrapAll = WrapU | WrapV | WrapW,
        MirrorAll = MirrorU | MirrorV | MirrorW,
        BorderAll = BorderU | BorderV | BorderW,
        ClampAll = ClampU | ClampV | ClampW,
        MirrorOnceAll = MirrorOnceU | MirrorOnceV | MirrorOnceW
    };

    struct SamplerOptions
    {
        std::optional<D3D11_FILTER> filter;
        std::optional<u16> textureAddressMode;
        std::optional<f32> mipLODBias;
        std::optional<UINT> maxAnisotropy;
        std::optional<D3D11_COMPARISON_FUNC> comparisonFunc;
        std::optional<DirectX::XMFLOAT4> borderColor;
        std::optional<f32> minLOD;
        std::optional<f32> maxLOD;
    };

    using Sampler = Microsoft::WRL::ComPtr<ID3D11SamplerState>;

    AQUAXP_API Result<Sampler> CreateSampler(ID3D11Device* device, SamplerOptions const& options = SamplerOptions());
}