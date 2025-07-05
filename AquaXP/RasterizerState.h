#pragma once

namespace AquaXP
{
#pragma warning( push )
#pragma warning( disable: 4251 )
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
#pragma warning( pop )

	class RasterizerState
	{
	public:
		AQUAXP_API RasterizerState(ID3D11Device* device, RasterizerStateDesc const& stateDesc);
		AQUAXP_API RasterizerState(ID3D11Device* device);

		AQUAXP_API Microsoft::WRL::ComPtr<ID3D11RasterizerState> getRasterizerState() const;
	private:
		bool m_status;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
	};
}