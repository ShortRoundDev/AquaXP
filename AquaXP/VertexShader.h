#pragma once
#include "framework.h"
#include <memory>
#include "FileHelpers.h"
#include "ShaderHelper.h"
#include "InputLayoutBuilder.h"

namespace AquaXP
{
    template<template<typename> typename Alloc = std::allocator>
    class VertexShader
    {
        using ByteAllocType = Alloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

    public:
        VertexShader(
            ID3D11Device* device,
            std::wstring const& path,
            std::pair<D3D11_INPUT_ELEMENT_DESC const*, UINT> const& inputLayout
        ) : VertexShader(
            device,
            path,
            get<0>(inputLayout),
            get<1>(inputLayout)
        )
        {
        }

        VertexShader(
            ID3D11Device* device,
            std::wstring const& path,
            D3D11_INPUT_ELEMENT_DESC const* inputLayoutElements,
            UINT numInputLayoutElements
        )
        {
            std::shared_ptr<u8[]> byteCode;
            sz byteCodeSize;
            if (!InitShaderCode<ID3D11VertexShader, Alloc>(
                &ID3D11Device::CreateVertexShader,
                device,
                path,
                byteCode,
                byteCodeSize,
                m_shader.GetAddressOf()
            ))
            {
                return;
            }

            if (!initInputLayout(
                device,
                inputLayoutElements,
                numInputLayoutElements,
                byteCode.get(),
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
            D3D11_INPUT_ELEMENT_DESC const* inputLayoutElements,
            UINT numInputLayoutElements,
            u8* byteCode,
            sz byteCodeLength
        )
        {
            auto res = device->CreateInputLayout(
                inputLayoutElements,
                numInputLayoutElements,
                byteCode,
                byteCodeLength,
                m_inputLayout.GetAddressOf()
            );

            if (FAILED(res))
            {
                return false;
            }
            return true;
        }

    };
}