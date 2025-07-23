#pragma once

#include <memory>
#include "ShaderHelper.h"
#include "StringHelpers.h"

namespace AquaXP
{
    class PixelShader
    {
    public:
        PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> shader) : m_shader(shader)
        { }

        void use(ID3D11DeviceContext* context) const
        {
            context->PSSetShader(m_shader.Get(), NULL, 0);
        }

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11PixelShader> getShader() const
        {
            return m_shader;
        }

    private:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
    };

    template<template<typename> typename Alloc = std::allocator>
    Result<PixelShader> LoadPixelShader(ID3D11Device* device,
        std::wstring const& path
    )
    {
        using ByteAllocType = Alloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

        Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
        
        std::shared_ptr<u8[]> byteCode;
        sz byteCodeSize;

        auto shaderInitResult = InitShaderCode<ID3D11PixelShader, Alloc>(
            &ID3D11Device::CreatePixelShader,
            device,
            path,
            byteCode,
            byteCodeSize,
            shader.GetAddressOf()
        );
        if(!isOk(shaderInitResult))
        {
            return error(shaderInitResult);
        }

        return PixelShader(shader);
    }

    template<template<typename> typename Alloc = std::allocator>
    Result<PixelShader> LoadPixelShader(ID3D11Device* device,
        std::string const& path
    )
    {
        using ByteAllocType = Alloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

        std::wstring wpath;
        if (!mbStrToWideChar(path, wpath))
        {
            return ErrorCode::WStringConversionFailure;
        }

        return LoadPixelShader(device, wpath);
    }
}
