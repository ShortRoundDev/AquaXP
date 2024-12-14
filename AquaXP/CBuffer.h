#pragma once
namespace AquaXP
{
    enum CbufferBindStage
    {
        VS,
        PS,
        GS,
        HS,
        CS,
        DS
    };

    template<typename T>
    class CBuffer
    {
    public:
        CBuffer(
            ID3D11Device* device,
            T const& cBufferData
        ) :
            m_cBufferData(cBufferData),
            m_cBuffer()
        {
            init(device);
        }

        bool bind(
            ID3D11DeviceContext* context,
            UINT slot = 0,
            CbufferBindStage bindStage = static_cast<CbufferBindStage>(VS | PS)
        ) const
        {
            T copy = { 0 };

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
                // TODO: Handle Error here
                return false;
            }

            void* localBuffer = (void*)bufferResource.pData;
            CopyMemory(localBuffer, &copy, sizeof(T));
            context->Unmap(m_cBuffer.Get(), 0);
            if (bindStage & VS)
            {
                context->VSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (bindStage & PS)
            {
                context->PSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (bindStage & GS)
            {
                context->GSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (bindStage & HS)
            {
                context->HSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (bindStage & CS)
            {
                context->CSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            if (bindStage & DS)
            {
                context->DSSetConstantBuffers(
                    slot, 1, m_cBuffer.GetAddressOf()
                );
            }
            return true;
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


    private:
        T m_cBufferData;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;

        bool init(ID3D11Device* device)
        {
            if (sizeof(T) == 0)
            {
                // Not actually an error, just stupid
                return true;
            }

            D3D11_BUFFER_DESC bufferDesc = {
                .ByteWidth = sizeof(T), // Use alignas(16) for cbuffers
                .Usage = D3D11_USAGE_DYNAMIC,
                .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
                .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
                .MiscFlags = 0,
                .StructureByteStride = 0
            };

            if (FAILED(device->CreateBuffer(
                &bufferDesc,
                NULL,
                m_cBuffer.GetAddressOf()
            )))
            {
                return false;
            }
            return true;
        }

    };
};
