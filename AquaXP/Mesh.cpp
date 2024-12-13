#include "pch.h"
#include "Mesh.h"

using namespace AquaXP;
using namespace std;

template<typename T>
class Mesh<T>::impl
{
public:
    impl(
        ID3D11Device const* device,
        T const* vertices,
        UINT vertexCount,
        UINT const* indices,
        UINT indexCount
    ):
        m_vertexBuffer(),
        m_indexBuffer(),
        m_indexCount(indexCount)
    {
        uploadVertexData(vertices, vertexCount, indices, indexCount);
    }

    void use(ID3D11DeviceContext* context, UINT slot = 0) const
    {
        UINT stride = sizeof(T);
        UINT offset = 0;

        context->IASetVertexBuffers(
            slot,
            1,
            m_vertexBuffer.GetAddressOf(),
            &stride,
            *offset
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
    )
    {
        D3D11_BUFFER_DESC vertexBufferDesc = {
            .Usage = D3D11_USAGE_DEFAULT,
            .ByteWidth = static_cast<UINT>(vertexCount * sizeof(T)),
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
            return;
        }

        D3D11_BUFFER_DESC indexBufferDesc = {
            .Usage = D3D11_USAGE_DEFAULT,
            .ByteWidth = static_cast<UINT>(indexCount * sizeof(T)),
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
            return;
        }
    }
};

template<typename T>
Mesh<T>::Mesh(
    ID3D11Device const* device,
    T const* vertices,
    UINT vertexCount,
    UINT const* indices,
    UINT indexCount
):
    m_pimpl(make_unique<Mesh<T>::impl>(device, vertices, vertexCount, indices, indexCount))
{

}

template<typename T>
Mesh<T>::Mesh(
    ID3D11Device const* device,
    vector<T> const& vertices,
    vector<UINT> const& indices
) :
    m_pimpl(make_unique<Mesh<T>::impl>(
        device,
        vertices.data(),
        vertices.size(),
        indices.data(),
        indices.size()
    ))
{

}

template<typename T>
Mesh<T>::Mesh(
    ID3D11Device const* device,
    std::pair<
        std::vector<T>,
        std::vector<UINT>
    > const& data
) :
    Mesh<T>::Mesh(
        device,
        get<0>(data),
        get<1>(data)
    )
{

}

template<typename T>
void Mesh<T>::use(
    ID3D11DeviceContext* device,
    UINT slot
) const
{
    m_pimpl->use(device, slot);
}

template<typename T>
void Mesh<T>::draw(
    ID3D11DeviceContext* device
) const
{
    m_pimpl->draw(device);
}

template<typename T>
void Mesh<T>::useAndDraw(
    ID3D11DeviceContext* device,
    UINT slot
) const
{
    m_pimpl->use(device, slot);
    m_pimpl->draw(device);
}