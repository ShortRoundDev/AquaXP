#pragma once

namespace AquaXP
{
#pragma warning( push )
#pragma warning( disable: 4251 )
	struct RasterizerStateOptions
	{
		std::optional<D3D11_FILL_MODE> fillMode;
		std::optional<D3D11_CULL_MODE> cullMode;
		std::optional<BOOL> frontCounterClockwise;
		std::optional<INT> depthBias;
		std::optional<FLOAT> depthBiasClamp;
		std::optional<FLOAT> slopeScaledDepthBias;
		std::optional<BOOL> depthClipEnable;
		std::optional<BOOL> scissorEnable;
		std::optional<BOOL> multisampleEnable;
		std::optional<BOOL> antialiasedLineEnable;
	};
#pragma warning( pop )

	using RasterizerState = Microsoft::WRL::ComPtr<ID3D11RasterizerState>;	

	AQUAXP_API Result<RasterizerState> CreateRasterizerState(
		ID3D11Device* device,
		RasterizerStateOptions const& options = RasterizerStateOptions()
	);
}