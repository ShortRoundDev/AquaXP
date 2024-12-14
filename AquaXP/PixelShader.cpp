#include "pch.h"
#include "PixelShader.h"
#include "ShaderHelper.h"

using namespace std;
using namespace AquaXP;

PixelShader::PixelShader(
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

void PixelShader::use(ID3D11DeviceContext* context) const
{
    context->PSSetShader(m_shader.Get(), NULL, 0);
}
