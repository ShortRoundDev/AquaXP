#pragma once

namespace AquaXP
{
	struct RasterizerStateDesc
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

	class RasterizerState
	{
	public:
		RasterizerState(ID3D11Device* device, RasterizerStateDesc const& stateDesc);
		RasterizerState(ID3D11Device* device);

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> getRasterizerState() const;
	private:
		bool m_status;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
	};
}