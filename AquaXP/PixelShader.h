#pragma once
namespace AquaXP
{
    class PixelShader
    {
    public:
        AQUAXP_API PixelShader(
            ID3D11Device* device,
            std::wstring const& path
        );

        AQUAXP_API void use(ID3D11DeviceContext* context) const;
    
    private:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
    };
}
