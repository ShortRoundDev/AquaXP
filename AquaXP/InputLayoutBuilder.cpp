#include "pch.h"
#include "InputLayoutBuilder.h"

using namespace AquaXP;

Result<Microsoft::WRL::ComPtr<ID3D11InputLayout>> AquaXP::BuildInputLayout(
    ID3D11Device* device,
    D3D11_INPUT_ELEMENT_DESC const* inputLayoutElements,
    UINT numInputLayoutElements,
    u8* byteCode,
    sz byteCodeLength
)
{
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

    HRESULT res = device->CreateInputLayout(
        inputLayoutElements,
        numInputLayoutElements,
        byteCode,
        byteCodeLength,
        inputLayout.GetAddressOf()
    );
    if (FAILED(res))
    {
        return HRToError(res);
    }
    return inputLayout;
}
