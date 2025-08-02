#pragma once

namespace AquaXP
{
#pragma warning( push )
#pragma warning( disable: 4251 )
	struct DepthStencilOpOptions {
		std::optional<D3D11_STENCIL_OP> stencilFailOp;
		std::optional<D3D11_STENCIL_OP> stencilDepthFailOp;
		std::optional<D3D11_STENCIL_OP> stencilPassOp;
		std::optional<D3D11_COMPARISON_FUNC> stencilFunc;
	};

	struct DepthStencilStateOptions
	{
		std::optional<BOOL> depthEnable;
		std::optional<D3D11_DEPTH_WRITE_MASK> depthWriteMask;
		std::optional<D3D11_COMPARISON_FUNC> depthFunc;
		std::optional<BOOL> stencilEnable;
		std::optional<UINT8> stencilReadMask;
		std::optional<UINT8> stencilWriteMask;
		std::optional<DepthStencilOpOptions> frontFace;
		std::optional<DepthStencilOpOptions> backFace;
	};
#pragma warning( pop )

	using DepthStencilState = Microsoft::WRL::ComPtr<ID3D11DepthStencilState>;

	AQUAXP_API Result<DepthStencilState> CreateDepthStencilState(
		ID3D11Device* device,
		DepthStencilStateOptions const& options = DepthStencilStateOptions()
	);
}