#pragma once

#include <memory>
#include "ShaderHelper.h"

namespace AquaXP
{
    template<template<typename> typename Alloc = std::allocator>
    class PixelShader
    {
        using ByteAllocType = Alloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");
    public:
        PixelShader(
            ID3D11Device* device,
            std::wstring const& path
        )
        {
            std::shared_ptr<u8[]> byteCode;
            sz byteCodeSize;
            if (!InitShaderCode<ID3D11PixelShader, Alloc>(
                &ID3D11Device::CreatePixelShader,
                device,
                path,
                byteCode,
                byteCodeSize,
                m_shader.GetAddressOf()
            ))
            {
                return;
            }
        }

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
    Result<PixelShader<Alloc>> LoadPixelShader(ID3D11Device* device,
        std::wstring const& path
    )
    {
        Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
        
        std::shared_ptr<u8[]> byteCode;
        sz byteCodeSize;

        if (!InitShaderCode<ID3D11PixelShader, Alloc>(
            &ID3D11Device::CreatePixelShader,
            device,
            path,
            byteCode,
            byteCodeSize,
            shader.GetAddressOf()
        ))
        {
            return ErrorCode::DepthBufferCreationFailed
        }
    }
}
