#include "pch.h"
#include "VertexShader.h"
#include "FileHelpers.h"
#include "ShaderHelper.h"

using namespace std;
using namespace AquaXP;

class VertexShader::impl
{
public:
    impl(
        ID3D11Device* device,
        wstring const& path,
        InputLayoutBuilder const& inputLayoutBuilder
    )
    {
        u8* byteCode;
        sz byteCodeSize;
        if (!InitShaderCode(
            &ID3D11Device::CreateVertexShader,
            device,
            path,
            &byteCode,
            &byteCodeSize,
            m_shader.GetAddressOf()
        ))
        {
            return;
        }

        if (!initInputLayout(
            device, 
            inputLayoutBuilder,
            byteCode,
            byteCodeSize
        ))
        {
            return;
        }
    }

    void use(ID3D11DeviceContext* context) const
    {
        context->IASetInputLayout(m_inputLayout.Get());
        context->VSSetShader(m_shader.Get(), NULL, 0);
    }

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    bool initInputLayout(
        ID3D11Device* device,
        InputLayoutBuilder const& inputLayoutBuilder,
        u8* byteCode,
        sz byteCodeLength
    )
    {
        auto builtLayoutDesc = inputLayoutBuilder.build();
        auto res = device->CreateInputLayout(
            get<0>(builtLayoutDesc),
            get<1>(builtLayoutDesc),
            byteCode,
            byteCodeLength,
            m_inputLayout.GetAddressOf()
        );

        delete byteCode;

        if (FAILED(res))
        {
            return false;
        }
        return true;
    }
};

VertexShader::VertexShader(
    ID3D11Device* device,
    wstring const& path,
    InputLayoutBuilder const& inputLayoutBuilder
) :
    m_pimpl(make_unique<VertexShader::impl>(device, path, inputLayoutBuilder))
{
}

void VertexShader::use(ID3D11DeviceContext* context) const
{
    m_pimpl->use(context);
}