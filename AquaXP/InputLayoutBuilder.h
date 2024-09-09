#pragma once
namespace AquaXP
{
    /// <summary>
    /// A builder class for creating input layouts.
    /// </summary>
    class InputLayoutBuilder
    {
    public:
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
        AQUAXP_API InputLayoutBuilder& addBinormal(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addBlendIndices(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_UINT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addBlendWeight(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addColor(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addNormal(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addPosition(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addTangent(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );

        AQUAXP_API InputLayoutBuilder& addTexCoord(
            UINT semanticIndex = 0,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT,
            UINT inputSlot = 0,
            std::optional<UINT> alignedByteOffset = std::nullopt,
            D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            UINT instanceDataStepRate = 0
        );
    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}