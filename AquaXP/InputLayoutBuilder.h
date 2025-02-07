#pragma once

#include "Concepts.h"

template<template<typename...> class Container>
concept InputElemContainer =
    IsAnyVector<Container<D3D11_INPUT_ELEMENT_DESC>> &&
    std::same_as<typename Container<D3D11_INPUT_ELEMENT_DESC>::value_type, D3D11_INPUT_ELEMENT_DESC>;

namespace AquaXP
{
    /// <summary>
    /// A builder class for creating input layouts.
    /// </summary>

    template<template<typename...> class Container = std::vector>
        requires InputElemContainer<Container>
    class InputLayoutBuilder
    {
    public:
        InputLayoutBuilder() :
            m_layoutBuffer() {}


        /// <summary>
        /// Adds a binormal to the input layout.
        /// </summary>
        /// <param name="semanticIndex">The index of the Binormal semantic. Default 0</param>
        /// <param name="format">The format of the Binormal. Default DXGI_FORMAT_R32G32B32_FLOAT</param>
        /// <param name="inputSlot"></param>
        /// <param name="alignedByteOffset"></param>
        /// <param name="inputSlotClass"></param>
        /// <param name="instanceDataStepRate"></param>
        /// <returns></returns>
        InputLayoutBuilder& addBinormal(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "BINORMAL",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }

        InputLayoutBuilder& addBlendIndices(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_UINT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "BLENDINDICES",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        InputLayoutBuilder& addBlendWeight(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "BLENDWEIGHT",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        InputLayoutBuilder& addColor(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "COLOR",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        InputLayoutBuilder& addNormal(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "NORMAL",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        InputLayoutBuilder& addPosition(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "POSITION",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        InputLayoutBuilder& addTangent(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "TANGENT",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        InputLayoutBuilder& addTexCoord(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        )
        {
            m_layoutBuffer.push_back({
                "TEXCOORD",
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                        : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }

        InputLayoutBuilder& add(
            char const* semantic,
            UINT semanticIndex,
            DXGI_FORMAT format,
            UINT inputSlot,
            std::optional<UINT> alignedByteOffset,
            D3D11_INPUT_CLASSIFICATION inputSlotClass,
            UINT instanceDataStepRate
        )
        {
            m_layoutBuffer.push_back({
                semantic,
                semanticIndex,
                format,
                inputSlot,
                alignedByteOffset.value_or(
                    m_layoutBuffer.empty()
                        ? 0
                    : D3D11_APPEND_ALIGNED_ELEMENT
                ),
                inputSlotClass,
                instanceDataStepRate
                });
            return *this;
        }


        std::pair<D3D11_INPUT_ELEMENT_DESC const*, UINT> build() const
        {
            return std::make_pair<D3D11_INPUT_ELEMENT_DESC const*, UINT>(
                m_layoutBuffer.data(),
                static_cast<UINT>(m_layoutBuffer.size())
            );
        }
    private:
        Container<D3D11_INPUT_ELEMENT_DESC> m_layoutBuffer;
    };
}
