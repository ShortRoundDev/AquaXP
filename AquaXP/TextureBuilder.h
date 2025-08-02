#pragma once

#include "Texture.h"

namespace AquaXP
{
	class TextureBuilder
	{
	public:
		TextureBuilder();

		TextureBuilder& withTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> const& texture);
		TextureBuilder& withTexture(TextureOptions const& options);
		TextureBuilder& withSRV(SRVOptions const& options = SRVOptions());
		TextureBuilder& withRTV(RTVOptions const& options = RTVOptions());
		TextureBuilder& withDSV(DSVOptions const& options);

		Result<Texture> build(ID3D11Device* device);
	private:
		std::optional<Microsoft::WRL::ComPtr<ID3D11Texture2D>> m_texture;
		std::optional<TextureOptions> m_textureOptions;
		std::optional<SRVOptions> m_srvOptions;
		std::optional<DSVOptions> m_dsvOptions;
		std::optional<RTVOptions> m_rtvOptions;
	};
}