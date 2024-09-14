#pragma once
class VertexShader
{
public:
    VertexShader(
        ID3D11Device* device,
        std::wstring const& path,
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout
    );

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
