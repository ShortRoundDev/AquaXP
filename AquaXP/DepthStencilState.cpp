#include "pch.h"
#include "DepthStencilState.h"

using namespace AquaXP;
using namespace DirectX;
using namespace std;

Result<DepthStencilState> AquaXP::CreateDepthStencilState(
	ID3D11Device* device,
	DepthStencilStateOptions const& options
)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription = { };
	ZeroMemory(&depthStencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDescription.DepthEnable = options.depthEnable.value_or(TRUE);
	depthStencilDescription.DepthWriteMask = options.depthWriteMask.value_or(D3D11_DEPTH_WRITE_MASK_ALL);
	depthStencilDescription.DepthFunc = options.depthFunc.value_or(D3D11_COMPARISON_LESS);
	depthStencilDescription.StencilEnable = options.stencilEnable.value_or(TRUE);
	depthStencilDescription.StencilReadMask = options.stencilReadMask.value_or(0xff);
	depthStencilDescription.StencilWriteMask = options.stencilWriteMask.value_or(0xff);

	auto frontFace = options.frontFace.value_or(DepthStencilOpOptions());
	depthStencilDescription.FrontFace.StencilFailOp = frontFace.stencilFailOp.value_or(D3D11_STENCIL_OP_KEEP);
	depthStencilDescription.FrontFace.StencilDepthFailOp = frontFace.stencilDepthFailOp.value_or(D3D11_STENCIL_OP_INCR);
	depthStencilDescription.FrontFace.StencilPassOp = frontFace.stencilPassOp.value_or(D3D11_STENCIL_OP_KEEP);
	depthStencilDescription.FrontFace.StencilFunc = frontFace.stencilFunc.value_or(D3D11_COMPARISON_ALWAYS);

	auto backFace = options.backFace.value_or(DepthStencilOpOptions());
	depthStencilDescription.BackFace.StencilFailOp = backFace.stencilFailOp.value_or(D3D11_STENCIL_OP_KEEP);
	depthStencilDescription.BackFace.StencilDepthFailOp = backFace.stencilDepthFailOp.value_or(D3D11_STENCIL_OP_DECR);
	depthStencilDescription.BackFace.StencilPassOp = backFace.stencilPassOp.value_or(D3D11_STENCIL_OP_KEEP);
	depthStencilDescription.BackFace.StencilFunc = backFace.stencilFunc.value_or(D3D11_COMPARISON_ALWAYS);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	auto res = device->CreateDepthStencilState(&depthStencilDescription, depthStencilState.GetAddressOf());
	if(FAILED(res))
	{
		return HRToError(res);
	}

	return depthStencilState;
}