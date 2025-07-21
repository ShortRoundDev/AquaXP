#include "pch.h"
#include "RasterizerState.h"

using namespace AquaXP;
using namespace std;

RasterizerState::RasterizerState(ID3D11Device* device, RasterizerStateDesc const& stateDesc) :
	m_status(true),
	m_rasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AntialiasedLineEnable = stateDesc.antialiasedLineEnable.value_or(TRUE);
	desc.CullMode = stateDesc.cullMode.value_or(D3D11_CULL_BACK);
	desc.DepthBias = stateDesc.depthBias.value_or(0);
	desc.DepthBiasClamp = stateDesc.depthBiasClamp.value_or(0.0f);
	desc.DepthClipEnable = stateDesc.depthClipEnable.value_or(TRUE);
	desc.FillMode = stateDesc.fillMode.value_or(D3D11_FILL_SOLID);
	desc.MultisampleEnable = stateDesc.multisampleEnable.value_or(TRUE);
	desc.ScissorEnable = stateDesc.scissorEnable.value_or(FALSE);
	desc.SlopeScaledDepthBias = stateDesc.slopeScaledDepthBias.value_or(0.0f);
	desc.FrontCounterClockwise = stateDesc.frontCounterClockwise.value_or(false);

	if (FAILED(device->CreateRasterizerState(&desc, m_rasterizerState.GetAddressOf()))) {
		m_status = false;
	}
}

RasterizerState::RasterizerState(ID3D11Device* device) :
	RasterizerState(device, {}) { }

Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState::getRasterizerState() const
{
	return m_rasterizerState;
}

