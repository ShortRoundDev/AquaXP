#pragma once

namespace AquaXP
{
    enum TextureAddressMode : u16
    {
        WRAP_U        = 0b0000000000000001,
        MIRROR_U      = 0b0000000000000010,
        BORDER_U      = 0b0000000000000100,
        CLAMP_U       = 0b0000000000001000,
        MIRROR_ONCE_U = 0b0000000000010000,
        
        WRAP_V        = 0b0000000000100000,
        MIRROR_V      = 0b0000000001000000,
        BORDER_V      = 0b0000000010000000,
        CLAMP_V       = 0b0000000100000000,
        MIRROR_ONCE_V = 0b0000001000000000,

        WRAP_W        = 0b0000010000000000,
        MIRROR_W      = 0b0000100000000000,
        BORDER_W      = 0b0001000000000000,
        CLAMP_W       = 0b0010000000000000,
        MIRROR_ONCE_W = 0b0100000000000000,
    };

    class Sampler
    {
    public:
        AQUAXP_API Sampler(
            ID3D11Device* device,
            D3D11_FILTER filterType = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
            TextureAddressMode textureAddressMode = static_cast<TextureAddressMode>(WRAP_U | WRAP_V | WRAP_W),
            DirectX::XMFLOAT4 border = DirectX::XMFLOAT4(0, 0, 0, 0)
        );

        AQUAXP_API void use(ID3D11DeviceContext* context, u32 slot = 0) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
    };
}