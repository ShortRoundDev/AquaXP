#include "pch.h"
#include "PixelShader.h"
#include "ShaderHelper.h"

using namespace std;
using namespace AquaXP;

class PixelShader::impl
{
public:
    impl(
        ID3D11Device* device,
        wstring const& path
    )
    {
        u8* byteCode;
        sz byteCodeSize;
        if (!InitShaderCode(
            &ID3D11Device::CreatePixelShader,
            device,
            path,
            &byteCode,
            &byteCodeSize,
            m_shader.GetAddressOf()
        ))
        {
            //TODO: Add error handling here
            return;
        }
    }

    void use(ID3D11DeviceContext* context) const
    {
        context->PSSetShader(m_shader.Get(), NULL, 0);
    }

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
};

PixelShader::PixelShader(
    ID3D11Device* device,
    wstring const& path
) : m_pimpl(make_unique<PixelShader::impl>(device, path))
{
    
}

void PixelShader::use(ID3D11DeviceContext* context) const
{
    m_pimpl->use(context);
}
