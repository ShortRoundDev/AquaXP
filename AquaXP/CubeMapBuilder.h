#pragma once

#include "CubeMap.h"
#include "Texture.h"

namespace AquaXP
{
	class CubeMapBuilder
	{
	public:
		CubeMapBuilder();
		CubeMapBuilder& withTexture(CubeMapTextureOptions const& options);
		CubeMapBuilder& withTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);
		CubeMapBuilder& withTexture(std::string const& path);
		CubeMapBuilder& withTexture(std::wstring const& path);
		CubeMapBuilder& withSRV(SRVOptions const& options = SRVOptions());
		CubeMapBuilder& withRTV(RTVOptions const& options = RTVOptions());
		CubeMapBuilder& withDSV(DSVOptions const& options);

		Result<CubeMap> build(ID3D11Device* device);

	private:
		/* Set when calling withTexture with a path - if there is an error with the path then this will be set,
		 * but the error won't be returned until `build()` is called. */
		std::optional<Result<Unit>> m_earlyError;
		std::optional<Microsoft::WRL::ComPtr<ID3D11Texture2D>> m_texture2D;
		/* Set if withTexture is called with a path */
		std::optional<std::wstring> m_path;
		std::optional<CubeMapTextureOptions> m_textureOptions;
		std::optional<SRVOptions> m_srvOptions;
		std::optional<DSVOptions> m_dsvOptions;
		std::optional<RTVOptions> m_rtvOptions;
	};
}