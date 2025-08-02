#pragma once

#include "Texture.h"

namespace AquaXP
{
	class TextureBuilder
	{
	public:
		AQUAXP_API TextureBuilder();

		AQUAXP_API TextureBuilder& withTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> const& texture);
		AQUAXP_API TextureBuilder& withTexture(TextureOptions const& options);
		AQUAXP_API TextureBuilder& withTexture(std::string const& path, TextureOptions const& options = TextureOptions());
		AQUAXP_API TextureBuilder& withTexture(std::wstring const& path, TextureOptions const& options = TextureOptions());
		AQUAXP_API TextureBuilder& withSRV(SRVOptions const& options = SRVOptions());
		AQUAXP_API TextureBuilder& withRTV(RTVOptions const& options = RTVOptions());
		AQUAXP_API TextureBuilder& withDSV(DSVOptions const& options);

		AQUAXP_API Result<Texture> build(ID3D11Device* device, ID3D11DeviceContext* context);
	private:
		std::optional<Result<Unit>> m_earlyError;
		std::optional<std::wstring> m_path;

		std::optional<Microsoft::WRL::ComPtr<ID3D11Texture2D>> m_texture;
		std::optional<TextureOptions> m_textureOptions;
		std::optional<SRVOptions> m_srvOptions;
		std::optional<DSVOptions> m_dsvOptions;
		std::optional<RTVOptions> m_rtvOptions;
	};
}