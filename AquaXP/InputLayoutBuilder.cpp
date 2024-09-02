#include "pch.h"
#include "InputLayoutBuilder.h"

using namespace AquaXP;

InputLayoutBuilder& InputLayoutBuilder::addBinormal(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addBlendIndices(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addBlendWeight(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addColor(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addNormal(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addPosition(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addTangent(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

InputLayoutBuilder& InputLayoutBuilder::addTexCoord(
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    std::optional<UINT> alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
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

