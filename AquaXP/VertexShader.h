#pragma once
#include "framework.h"
#include <memory>
#include "FileHelpers.h"
#include "ShaderHelper.h"
#include "InputLayoutBuilder.h"
#include "StringHelpers.h"

namespace AquaXP
{
    class VertexShader
    {
    public:

        AQUAXP_API VertexShader(
            Microsoft::WRL::ComPtr<ID3D11VertexShader> shader,
            Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout
        );

        AQUAXP_API void use(ID3D11DeviceContext* context);
        [[nodiscard]] AQUAXP_API Microsoft::WRL::ComPtr<ID3D11VertexShader> getShader() const;
        [[nodiscard]] AQUAXP_API Microsoft::WRL::ComPtr<ID3D11InputLayout> getInputLayout() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    };


    template<template<typename> typename Alloc = std::allocator>
    Result<VertexShader> LoadVertexShader(
        ID3D11Device* device,
        std::wstring const& path,
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout
    )
    {
        using ByteAllocType = Alloc<u8>;
        using ByteAllocTraits = std::allocator_traits<ByteAllocType>;
        static_assert(std::is_same_v<typename ByteAllocTraits::value_type, u8>,
            "Byte Allocator must be for u8 type");

        std::shared_ptr<u8[]> byteCode;
        sz byteCodeSize;

        auto shaderInitResult = InitShaderCode<ID3D11VertexShader, Alloc>(
            &ID3D11Device::CreateVertexShader,
            device,
            path,
            byteCode,
            byteCodeSize
        );

        if (!isOk(shaderInitResult))
        {
            return error(shaderInitResult);
        }

        return VertexShader(get(shaderInitResult), inputLayout);
    }


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

        std::shared_ptr<u8[]> byteCode;
        sz byteCodeSize;

        auto shaderInitResult = InitShaderCode<ID3D11VertexShader, Alloc>(
            &ID3D11Device::CreateVertexShader,
            device,
            path,
            byteCode,
            byteCodeSize
        );

        if(!isOk(shaderInitResult))
        {
            return error(shaderInitResult);
        }

        auto inputInitResult = BuildInputLayout(
            device,
            inputLayoutElements,
            numInputLayoutElements,
            byteCode.get(),
            byteCodeSize
        );

        if(!isOk(inputInitResult))
        {
            return error(inputInitResult);
        }
        
        return VertexShader(get(shaderInitResult), get(inputInitResult));
    }

    template<template<typename> typename Alloc = std::allocator>
    Result<VertexShader> LoadVertexShader(
        ID3D11Device* device,
        std::wstring const& path,
        std::pair<D3D11_INPUT_ELEMENT_DESC const*, UINT> const& inputLayout
    )
    {
        return LoadVertexShader<Alloc>(
            device,
            path,
            std::get<0>(inputLayout),
            std::get<1>(inputLayout)
        );
    }

    template<template<typename> typename Alloc = std::allocator>
    Result<VertexShader> LoadVertexShader(
        ID3D11Device* device,
        std::string const& path,
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout
    )
    {
        std::wstring wpath;
        if (!mbStrToWideChar(path, wpath))
        {
            return ErrorCode::WStringConversionFailure;
        }

        return LoadVertexShader(
            device,
            wpath,
            inputLayout
        );
    }

    template<template<typename> typename Alloc = std::allocator>
    Result<VertexShader> LoadVertexShader(
        ID3D11Device* device,
        std::string const& path,
        D3D11_INPUT_ELEMENT_DESC const* inputLayoutElements,
        UINT numInputLayoutElements
    )
    {
        std::wstring wpath;
        if (!mbStrToWideChar(path, wpath))
        {
            return ErrorCode::WStringConversionFailure;
        }

        return LoadVertexShader(
            device,
            wpath,
            inputLayoutElements,
            numInputLayoutElements
        );
    }

    template<template<typename> typename Alloc = std::allocator>
    Result<VertexShader> LoadVertexShader(
        ID3D11Device* device,
        std::string const& path,
        std::pair<D3D11_INPUT_ELEMENT_DESC const*, UINT> const& inputLayout
    )
    {
        return LoadVertexShader(
            device,
            path,
            std::get<0>(inputLayout),
            std::get<1>(inputLayout)
        );
    }
}
