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

    template<
        typename T,
        template<typename> typename ByteAlloc = std::allocator
    >
    bool InitShaderCode(
        Initializer<T> initializer,
        ID3D11Device* device,
        std::wstring const& path,
        std::shared_ptr<u8[]>& byteCode,
        sz& byteCodeSize,
        T** container
    )
    {
        using ByteAllocType = ByteAlloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

        if (!TryReadFile<ByteAlloc>((WCHAR*)path.c_str(), byteCode, byteCodeSize))
        {
            return false;
        }

        HRESULT res;
        res = (device->*initializer)(
            byteCode.get(),
            byteCodeSize,
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