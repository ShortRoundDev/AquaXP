#pragma once

#include "FileHelpers.h"

namespace AquaXP
{
    template<typename T>
    using Initializer = HRESULT(ID3D11Device::*)(
        void const* byteCode,
        sz byteCodeSize,
        ID3D11ClassLinkage* classLinkage,
        T** container
    );

    template<typename T>
    bool InitShaderCode(
        Initializer<T> initializer,
        ID3D11Device* device,
        std::wstring const& path,
        u8** byteCode,
        sz* byteCodeSize,
        T** container
    )
    {
        if (!TryReadFile(path.c_str(), byteCode, byteCodeSize))
        {
            return false;
        }

        HRESULT res;
        res = (device->*initializer)(
            *byteCode,
            *byteCodeSize,
            nullptr,
            container
            );
        if (FAILED(res))
        {
            return false;
        }
        return true;
    }
}