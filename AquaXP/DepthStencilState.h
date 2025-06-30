#pragma once

namespace AquaXP
{

	struct DepthStencilOpDesc {
		std::optional<D3D11_STENCIL_OP> stencilFailOp;
		std::optional<D3D11_STENCIL_OP> stencilDepthFailOp;
		std::optional<D3D11_STENCIL_OP> stencilPassOp;
		std::optional<D3D11_COMPARISON_FUNC> stencilFunc;
	};

	struct DepthStencilStateDesc
	{
		std::optional<BOOL> depthEnable;
		std::optional<D3D11_DEPTH_WRITE_MASK> depthWriteMask;
		std::optional<D3D11_COMPARISON_FUNC> depthFunc;
		std::optional<BOOL> stencilEnable;
		std::optional<UINT8> stencilReadMask;
		std::optional<UINT8> stencilWriteMask;
		std::optional<DepthStencilOpDesc> frontFace;
		std::optional<DepthStencilOpDesc> backFace;
	};


	class DepthStencilState
	{
	public:
		DepthStencilState(ID3D11Device* device, DepthStencilStateDesc const& depthStencilStateDesc);
		DepthStencilState(ID3D11Device* device);
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> getDepthStencilState() const;

	private:
		bool m_status;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	};
}