#pragma once
namespace AquaXP
{
    template<typename T>
    class CBuffer
    {
    public:
        CBuffer(
            T const& data,
            Microsoft::WRL::ComPtr<ID3D11Buffer> buffer
        ) : m_cBufferData(data),
            m_cBuffer(buffer)
        { }

        Result<Unit> bind(
            ID3D11DeviceContext* context,
            UINT slot = 0,
            Bind bind = Bind::VS | Bind::PS
        ) const
        {
            D3D11_MAPPED_SUBRESOURCE bufferResource;
            HRESULT res = context->Map(
                m_cBuffer.Get(),
                0,
                D3D11_MAP_WRITE_DISCARD,
                0,
                &bufferResource
            );

            if (FAILED(res))
            {
                return HRToError(res);
            }

            void* localBuffer = (void*)bufferResource.pData;
            CopyMemory(localBuffer, &m_cBufferData, sizeof(T));
            context->Unmap(m_cBuffer.Get(), 0);
            if (HasFlag(bind, Bind::VS))
            {
                context->VSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (HasFlag(bind, Bind::PS))
            {
                context->PSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (HasFlag(bind, Bind::GS))
            {
                context->GSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (HasFlag(bind, Bind::HS))
            {
                context->HSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (HasFlag(bind, Bind::CS))
            {
                context->CSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (HasFlag(bind, Bind::DS))
            {
                context->DSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }

            return Unit{};
        }

        void setData(T const& data)
        {
            m_cBufferData = data;
        }

        T const& getData() const
        {
            return m_cBufferData;
        }

        T& getDataMutable()
        {
            return m_cBufferData;
        }

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Buffer> getBuffer() const
        {
            return m_cBuffer;
        }

    private:
        T m_cBufferData;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;
    };

    struct CBufferOptions
    {
        std::optional<D3D11_USAGE> usage;// = D3D11_USAGE_DYNAMIC;
        std::optional<UINT> bindFlags;// = D3D11_BIND_CONSTANT_BUFFER;
        std::optional<UINT> cpuAccessFlags;// = D3D11_CPU_ACCESS_WRITE;
        std::optional<UINT> miscFlags;// = 0;
        std::optional<UINT> structureByteStride;// = 0;
    };

    template<typename T>
    Result<CBuffer<T>> CreateCBuffer(ID3D11Device* device, T const& initialData, CBufferOptions options = CBufferOptions())
    {
        if (sizeof(T) == 0)
        {
            return ErrorCode::EmptyCBuffer;
        }

        Microsoft::WRL::ComPtr<ID3D11Buffer> cBuffer;

        D3D11_BUFFER_DESC bufferDesc = {
            .ByteWidth = sizeof(T),
            .Usage = options.usage.value_or(D3D11_USAGE_DYNAMIC),
            .BindFlags = options.bindFlags.value_or(D3D11_BIND_CONSTANT_BUFFER),
            .CPUAccessFlags = options.cpuAccessFlags.value_or(D3D11_CPU_ACCESS_WRITE),
            .MiscFlags = options.miscFlags.value_or(0),
            .StructureByteStride = options.structureByteStride.value_or(0)
        };

        HRESULT res = device->CreateBuffer(
            &bufferDesc,
            NULL,
            cBuffer.GetAddressOf()
        );

        if (FAILED(res))
        {
            return HRToError(res);
        }
        return CBuffer<T>(initialData, cBuffer);
    }
};
