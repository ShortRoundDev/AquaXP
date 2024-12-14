#pragma once

namespace AquaXP
{
    template<typename T>
    class Mesh
    {
    public:
        AQUAXP_API Mesh(
            ID3D11Device const* device,
            T const* vertices,
            UINT vertexCount,
            UINT const* indices,
            UINT indexCount
        );

        AQUAXP_API Mesh(
            ID3D11Device const* device,
            std::vector<T> const& vertices,
            std::vector<UINT > const& indices
        );

        AQUAXP_API Mesh(
            ID3D11Device const* device,
            std::pair<
                std::vector<T>,
                std::vector<UINT>
            > const& data
        );

        AQUAXP_API void use(ID3D11DeviceContext* device, UINT slot = 0) const;
        AQUAXP_API void draw(ID3D11DeviceContext* device) const;
        AQUAXP_API void useAndDraw(ID3D11DeviceContext* device, UINT slot = 0) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
        u32 m_indexCount;

        bool uploadVertexData(
            ID3D11Device const* device,
            T const* vertices,
            UINT vertexCount,
            UINT const* indices,
            UINT indexCount
        );
    };
}
