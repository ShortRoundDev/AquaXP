#pragma once

namespace AquaXP
{
    struct CubeMapTextureOptions
    {
        u32 width;
        u32 height;
        DXGI_FORMAT format;
        std::optional<UINT> bindFlags;
        std::optional<UINT> mipLevels;
        std::optional<D3D11_USAGE> usage;
        std::optional<UINT> cpuAccessFlags;
        std::optional<UINT> miscFlags;
    };

    class CubeMap
    {
    public:

        AQUAXP_API CubeMap(
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D,
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView,
            std::optional<std::array<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, 6>> renderTargetViews,
            std::optional<std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6>> depthStencilViews
        );

        AQUAXP_API void use(ID3D11DeviceContext* context, u32 const slot = TEX_SKYBOX);
        AQUAXP_API Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;
        AQUAXP_API std::optional<std::reference_wrapper<const std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6>>> getDepthStencilViews() const;
        AQUAXP_API std::optional<std::reference_wrapper<const std::array<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, 6>>> getRenderTargetViews() const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
        std::optional<std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6>> m_depthStencilViews;
        std::optional<std::array<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, 6>> m_renderTargetViews;
    };

    enum class CubeMapFace
    {
        Right = 0,
        Left = 1,
        Up = 2,
        Down = 3,
        Front = 4,
        Back = 5
    };

    consteval std::pair<DirectX::XMVECTOR, DirectX::XMVECTOR> GetCubeMapLook(CubeMapFace face)
    {
        switch (face)
        {
        case CubeMapFace::Right:
        {
            return std::make_pair(
                DirectX::XMVectorSet(1, 0, 0, 0),
                DirectX::XMVectorSet(0, 1, 0, 0)
            );
        }
        case CubeMapFace::Left:
        {
            return std::make_pair(
                DirectX::XMVectorSet(-1, 0, 0, 0),
                DirectX::XMVectorSet(0, 1, 0, 0)
            );
        }
        case CubeMapFace::Up:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, 1, 0, 0),
                DirectX::XMVectorSet(0, 0, -1, 0)
            );
        }
        case CubeMapFace::Down:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, -1, 0, 0),
                DirectX::XMVectorSet(0, 0, 1, 0)
            );
        }
        case CubeMapFace::Front:
        {
            return std::make_pair(
                DirectX::XMVectorSet(0, 0, 1, 0),
                DirectX::XMVectorSet(0, 0, 1, 0)
            );
        }
        case CubeMapFace::Back:
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

    Result<CubeMap> LoadCubemap(ID3D11Device* device, std::string const& path);
    Result<CubeMap> LoadCubemap(ID3D11Device* device, std::wstring const& path);
}
