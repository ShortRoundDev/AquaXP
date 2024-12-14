#pragma once

#include "InputLayoutBuilder.h"

namespace AquaXP
{
    class VertexShader
    {
    public:
        AQUAXP_API VertexShader(
            ID3D11Device* device,
            std::wstring const& path,
            InputLayoutBuilder const& inputLayoutBuilder
        );

        AQUAXP_API void use(ID3D11DeviceContext* context) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

        bool initInputLayout(
            ID3D11Device* device,
            InputLayoutBuilder const& inputLayoutBuilder,
            u8* byteCode,
            sz byteCodeLength
        );
    };
}