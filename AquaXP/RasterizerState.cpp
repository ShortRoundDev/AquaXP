#include "pch.h"
#include "RasterizerState.h"

using namespace AquaXP;
using namespace std;

Result<RasterizerState> AquaXP::CreateRasterizerState(ID3D11Device* device, RasterizerStateOptions const& options)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AntialiasedLineEnable = options.antialiasedLineEnable.value_or(TRUE);
	desc.CullMode = options.cullMode.value_or(D3D11_CULL_BACK);
	desc.DepthBias = options.depthBias.value_or(0);
	desc.DepthBiasClamp = options.depthBiasClamp.value_or(0.0f);
	desc.DepthClipEnable = options.depthClipEnable.value_or(TRUE);
	desc.FillMode = options.fillMode.value_or(D3D11_FILL_SOLID);
	desc.MultisampleEnable = options.multisampleEnable.value_or(TRUE);
	desc.ScissorEnable = options.scissorEnable.value_or(FALSE);
	desc.SlopeScaledDepthBias = options.slopeScaledDepthBias.value_or(0.0f);
	desc.FrontCounterClockwise = options.frontCounterClockwise.value_or(false);

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	auto res = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
	if (FAILED(res))
	{
		return HRToError(res);
	}

	return rasterizerState;
}
