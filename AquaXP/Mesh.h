#pragma once

namespace AquaXP
{
    template<typename T>
    class Mesh
    {
    public:
        Mesh(
            ID3D11Device* device,
            T const* vertices,
            UINT vertexCount,
            UINT const* indices,
            UINT indexCount
        ) :
            m_vertexBuffer(),
            m_indexBuffer(),
            m_indexCount(indexCount)
        {
            uploadVertexData(device, vertices, vertexCount, indices, indexCount);
        }

        Mesh(
            ID3D11Device* device,
            std::vector<T> const& vertices,
            std::vector<UINT> const& indices
        ) :
            Mesh(
                device,
                vertices.data(),
                static_cast<UINT>(vertices.size()),
                indices.data(),
                static_cast<UINT>(indices.size())
            ) { }

        Mesh(
            ID3D11Device* device,
            std::initializer_list<T> const& vertices,
            std::initializer_list<UINT> const& indices
        ) :
            Mesh(
                device,
                std::vector<T>(vertices.begin(), vertices.end()),
                std::vector<UINT>(indices.begin(), indices.end())
            ) { }

        Mesh(
            ID3D11Device* device,
            std::pair<
                std::vector<T>,
                std::vector<UINT>
            > const& data
        ) : Mesh<T>::Mesh(
                device,
                get<0>(data),
                get<1>(data)
            ) { }

        void use(ID3D11DeviceContext* context, UINT slot = 0) const
        {
            UINT stride = sizeof(T);
            UINT offset = 0;

            context->IASetVertexBuffers(
                slot,
                1,
                m_vertexBuffer.GetAddressOf(),
                &stride,
                &offset
            );

            context->IASetIndexBuffer(
                m_indexBuffer.Get(),
                DXGI_FORMAT_R32_UINT,
                0
            );

            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }

        void draw(ID3D11DeviceContext* context) const
        {
            context->DrawIndexed(m_indexCount, 0, 0);
        }

        void useAndDraw(ID3D11DeviceContext* context, UINT slot = 0) const
        {
            use(context, slot);
            draw(context);
        }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
        u32 m_indexCount;

        bool uploadVertexData(
            ID3D11Device* device,
            T const* vertices,
            UINT vertexCount,
            UINT const* indices,
            UINT indexCount
        )
        {
            D3D11_BUFFER_DESC vertexBufferDesc = {
                .ByteWidth = static_cast<UINT>(vertexCount * sizeof(T)),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_VERTEX_BUFFER,
                .CPUAccessFlags = 0,
                .MiscFlags = 0,
                .StructureByteStride = 0
            };

            D3D11_SUBRESOURCE_DATA vertexBufferData = {
                .pSysMem = vertices,
                .SysMemPitch = 0,
                .SysMemSlicePitch = 0
            };

            HRESULT res = device->CreateBuffer(
                &vertexBufferDesc,
                &vertexBufferData,
                m_vertexBuffer.GetAddressOf()
            );

            if (FAILED(res))
            {
                return false;
            }

            D3D11_BUFFER_DESC indexBufferDesc = {
                .ByteWidth = static_cast<UINT>(indexCount * sizeof(T)),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_INDEX_BUFFER,
                .CPUAccessFlags = 0,
                .MiscFlags = 0,
                .StructureByteStride = 0
            };

            D3D11_SUBRESOURCE_DATA indexBufferData = {
                .pSysMem = indices,
                .SysMemPitch = 0,
                .SysMemSlicePitch = 0
            };

            res = device->CreateBuffer(
                &indexBufferDesc,
                &indexBufferData,
                m_indexBuffer.GetAddressOf()
            );

            if (FAILED(res))
            {
                // TODO: Add error handling here
                return false;
            }
            return true;
        }
    };
}
