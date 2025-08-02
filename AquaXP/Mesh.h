#pragma once

namespace AquaXP
{
    template<typename T>
    class Mesh
    {
    public:

        Mesh(
            Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer,
            Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer,
            UINT indexCount
        ) : m_vertexBuffer(vertexBuffer),
            m_indexBuffer(indexBuffer),
            m_indexCount(indexCount)
        { }

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
    };

    struct BufferOptions
    {
        std::optional<UINT> byteWidth;
        std::optional<D3D11_USAGE> usage;
        std::optional<UINT> bindFlags;
        std::optional<UINT> cpuAccessFlags;
        std::optional<UINT> miscFlags;
        std::optional<UINT> structureByteStride;
    };

    struct SubresourceDataOptions
    {
        std::optional<UINT> sysMemPitch;
        std::optional<UINT> sysMemSlicePitch;
    };

    struct MeshOptions
    {
        std::optional<BufferOptions> vertexBuffer;
        std::optional<SubresourceDataOptions> vertexData;
        std::optional<BufferOptions> indexBuffer;
        std::optional<SubresourceDataOptions> indexData;
    };

    template<typename T>
    Result<Mesh<T>> CreateMesh(
        ID3D11Device* device,
        T const* vertices,
        UINT vertexCount,
        UINT const* indices,
        UINT indexCount,
        MeshOptions options = MeshOptions()
    )
    {
        auto vertexBufferOptions = options.vertexBuffer.value_or(BufferOptions());
        D3D11_BUFFER_DESC vertexBufferDesc = {
            .ByteWidth = vertexBufferOptions.byteWidth.value_or(static_cast<UINT>(vertexCount * sizeof(T))),
            .Usage = vertexBufferOptions.usage.value_or(D3D11_USAGE_DEFAULT),
            .BindFlags = vertexBufferOptions.bindFlags.value_or(D3D11_BIND_VERTEX_BUFFER),
            .CPUAccessFlags = vertexBufferOptions.cpuAccessFlags.value_or(0),
            .MiscFlags = vertexBufferOptions.miscFlags.value_or(0),
            .StructureByteStride = vertexBufferOptions.structureByteStride.value_or(0)
        };

        auto vertexDataOptions = options.vertexData.value_or(SubresourceDataOptions());
        D3D11_SUBRESOURCE_DATA vertexBufferData = {
            .pSysMem = vertices,
            .SysMemPitch = vertexDataOptions.sysMemPitch.value_or(0),
            .SysMemSlicePitch = vertexDataOptions.sysMemSlicePitch.value_or(0)
        };

        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
        HRESULT res = device->CreateBuffer(
            &vertexBufferDesc,
            &vertexBufferData,
            vertexBuffer.GetAddressOf()
        );

        if (FAILED(res))
        {
            return HRToError(res);
        }

        auto indexBufferOptions = options.indexBuffer.value_or(BufferOptions());
        D3D11_BUFFER_DESC indexBufferDesc = {
            .ByteWidth = indexBufferOptions.byteWidth.value_or(static_cast<UINT>(indexCount * sizeof(UINT))),
            .Usage = indexBufferOptions.usage.value_or(D3D11_USAGE_DEFAULT),
            .BindFlags = indexBufferOptions.bindFlags.value_or(D3D11_BIND_INDEX_BUFFER),
            .CPUAccessFlags = indexBufferOptions.cpuAccessFlags.value_or(0),
            .MiscFlags = indexBufferOptions.miscFlags.value_or(0),
            .StructureByteStride = indexBufferOptions.structureByteStride.value_or(0)
        };

        auto indexDataOptions = options.indexData.value_or(SubresourceDataOptions());
        D3D11_SUBRESOURCE_DATA indexBufferData = {
            .pSysMem = indices,
            .SysMemPitch = indexDataOptions.sysMemPitch.value_or(0),
            .SysMemSlicePitch = indexDataOptions.sysMemSlicePitch.value_or(0)
        };

        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
        res = device->CreateBuffer(
            &indexBufferDesc,
            &indexBufferData,
            indexBuffer.GetAddressOf()
        );

        if (FAILED(res))
        {
            // TODO: Add error handling here
            return HRToError(res);
        }

        return Mesh<T>(vertexBuffer, indexBuffer, indexCount);
    }

    template<typename T, typename VertexContainer, typename IndexContainer>
        requires
        std::ranges::contiguous_range<VertexContainer>&& std::ranges::contiguous_range<IndexContainer>&&
        std::same_as<std::ranges::range_value_t<VertexContainer>, T>&&
        std::same_as<std::ranges::range_value_t<IndexContainer>, UINT>
        Result<Mesh<T>> CreateMesh(
            ID3D11Device* device,
            VertexContainer const& vertices,
            IndexContainer const& indices,
            MeshOptions options = MeshOptions()
        )
    {
        return CreateMesh(
            device,
            std::ranges::data(vertices),
            static_cast<UINT>(std::distance(vertices.begin(), vertices.end())),
            std::ranges::data(indices),
            static_cast<UINT>(std::distance(indices.begin(), indices.end())),
            options
        );
    }

    template<typename T>
    Result<Mesh<T>> CreateMesh(
        ID3D11Device* device,
        std::initializer_list<T> const& vertices,
        std::initializer_list<UINT> const& indices
    )
    {
        return CreateMesh(
            device,
            std::ranges::data(vertices),
            static_cast<UINT>(std::distance(vertices.begin(), vertices.end())),
            std::ranges::data(indices),
            static_cast<UINT>(std::distance(indices.begin(), indices.end()))
        );
    }

    template<typename T, typename VertexContainer, typename IndexContainer>
        requires
    std::ranges::contiguous_range<VertexContainer>&& std::ranges::contiguous_range<IndexContainer>&&
        std::same_as<std::ranges::range_value_t<VertexContainer>, T>&&
        std::same_as<std::ranges::range_value_t<IndexContainer>, UINT>
        Result<Mesh<T>> CreateMesh(
            ID3D11Device* device,
            std::pair<VertexContainer, IndexContainer> const& data
        )
    {
        return CreateMesh(
            device,
            get<0>(data),
            get<1>(data)
        );
    }

}
