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
        AQUAXP_API CBuffer(
            ID3D11Device* device,
            T const& cbufferData
        );

        AQUAXP_API bool bind(
            ID3D11DeviceContext* context,
            UINT slot = 0,
            CbufferBindStage bindStage = static_cast<CbufferBindStage>(VS | PS)
        ) const;

        AQUAXP_API void setData(T const& data);
        AQUAXP_API T const& getData() const;
        AQUAXP_API T& getDataMutable();

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
};
