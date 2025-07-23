#pragma once
#include "framework.h"
#include <memory>
#include "FileHelpers.h"
#include "ShaderHelper.h"
#include "InputLayoutBuilder.h"

namespace AquaXP
{
    class VertexShader
    {
    public:

        VertexShader(
            Microsoft::WRL::ComPtr<ID3D11VertexShader> shader,
            Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout
        ) :
            m_shader(shader),
            m_inputLayout(inputLayout)
        { }

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

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11VertexShader> getShader() const
        {
            return m_shader;
        }

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11InputLayout> getInputLayout() const
        {
            return m_inputLayout;
        }

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    };

    template<template<typename> typename Alloc = std::allocator>
    Result<VertexShader> LoadVertexShader(
        ID3D11Device* device,
        std::wstring const& path,
        D3D11_INPUT_ELEMENT_DESC const* inputLayoutElements,
        UINT numInputLayoutElements
    )
    {
        using ByteAllocType = Alloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");
        
        Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

        std::shared_ptr<u8[]> byteCode;
        sz byteCodeSize;

        auto shaderInitResult = InitShaderCode<ID3D11VertexShader, Alloc>(
            &ID3D11Device::CreateVertexShader,
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

        auto inputInitResult = initInputLayout(
            device,
            inputLayoutElements,
            numInputLayoutElements,
            byteCode.get(),
            byteCodeSize
        );
        {
            return;
        }
    }
}