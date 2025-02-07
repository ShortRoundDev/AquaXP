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
    };

    class Sampler
    {
    public:
        AQUAXP_API Sampler(
            ID3D11Device* device,
            D3D11_FILTER filterType = D3D11_FILTER_ANISOTROPIC,
            TextureAddressMode textureAddressMode = static_cast<TextureAddressMode>(ClampU | ClampV | ClampW),
            DirectX::XMFLOAT4 border = DirectX::XMFLOAT4(0, 0, 0, 0)
        );

        AQUAXP_API void use(ID3D11DeviceContext* context, u32 slot = 0) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
    };
}