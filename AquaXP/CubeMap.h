#pragma once

namespace AquaXP
{
    class CubeMap
    {
    public:
        AQUAXP_API CubeMap(ID3D11Device* device, std::wstring const& path);
        AQUAXP_API CubeMap(ID3D11Device* device, WCHAR const* path);

        AQUAXP_API CubeMap(ID3D11Device* device, u16 width, u16 height);

        AQUAXP_API void use(ID3D11DeviceContext* context, u32 const slot = TEX_SKYBOX);
        AQUAXP_API std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6> const& getDepthStencilViews() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
        std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6> m_shadowMapFacesDsv;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthMapTexture2D;
    };

    enum class PointShadowFace
    {
        RIGHT = 0,
        LEFT = 1,
        UP = 2,
        DOWN = 3,
        FRONT = 4,
        BACK = 5
    };

    consteval std::pair<DirectX::XMVECTOR, DirectX::XMVECTOR> GetPointShadowMapLook(PointShadowFace face)
    {
        switch (face)
        {
        case PointShadowFace::RIGHT:
        {
            return std::make_pair(
                DirectX::XMVectorSet(1, 0, 0, 0),
                DirectX::XMVectorSet(0, 1, 0, 0)
            );
        }
        case PointShadowFace::LEFT:
        {
            return std::make_pair(
                DirectX::XMVectorSet(-1, 0, 0, 0),
                DirectX::XMVectorSet(0, 1, 0, 0)
            );
        }
        case PointShadowFace::UP:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, 1, 0, 0),
                DirectX::XMVectorSet(0, 0, -1, 0)
            );
        }
        case PointShadowFace::DOWN:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, -1, 0, 0),
                DirectX::XMVectorSet(0, 0, 1, 0)
            );
        }
        case PointShadowFace::FRONT:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, 0, 1, 0),
                DirectX::XMVectorSet(0, 0, 1, 0)
            );
        }
        case PointShadowFace::BACK:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, 0, -1, 0),
                DirectX::XMVectorSet(0, 0, 1, 0)
            );
        }
        }
        /* Error mode */
        return std::make_pair(
            DirectX::XMVectorSet(0, 0, 0, 0),
            DirectX::XMVectorSet(0, 0, 0, 0)
        );
    }
}
