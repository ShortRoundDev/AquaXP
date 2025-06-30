#include "pch.h"
#include "DepthStencilState.h"

using namespace AquaXP;
using namespace DirectX;
using namespace std;

DepthStencilState::DepthStencilState(
	ID3D11Device* device,
	DepthStencilStateDesc const& depthStencilStateDesc
) : m_depthStencilState(),
	m_status(true)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription = { };
	ZeroMemory(&depthStencilDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDescription.DepthEnable = depthStencilStateDesc.depthEnable.value_or(TRUE);
	depthStencilDescription.DepthWriteMask = depthStencilStateDesc.depthWriteMask.value_or(D3D11_DEPTH_WRITE_MASK_ALL);
	depthStencilDescription.DepthFunc = depthStencilStateDesc.depthFunc.value_or(D3D11_COMPARISON_LESS);
	depthStencilDescription.StencilEnable = depthStencilStateDesc.stencilEnable.value_or(TRUE);
	depthStencilDescription.StencilReadMask = depthStencilStateDesc.stencilReadMask.value_or(0xff);
	depthStencilDescription.StencilWriteMask = depthStencilStateDesc.stencilWriteMask.value_or(0xff);

	if (!depthStencilStateDesc.frontFace.has_value())
	{
		depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
	else
	{
		auto frontFace = depthStencilStateDesc.frontFace.value();
		depthStencilDescription.FrontFace.StencilFailOp = frontFace.stencilFailOp.value_or(D3D11_STENCIL_OP_KEEP);
		depthStencilDescription.FrontFace.StencilDepthFailOp = frontFace.stencilDepthFailOp.value_or(D3D11_STENCIL_OP_INCR);
		depthStencilDescription.FrontFace.StencilPassOp = frontFace.stencilPassOp.value_or(D3D11_STENCIL_OP_KEEP);
		depthStencilDescription.FrontFace.StencilFunc = frontFace.stencilFunc.value_or(D3D11_COMPARISON_ALWAYS);
	}

	if (!depthStencilStateDesc.backFace.has_value())
	{
		depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
	else
	{
		auto backFace = depthStencilStateDesc.backFace.value();
		depthStencilDescription.BackFace.StencilFailOp = backFace.stencilFailOp.value_or(D3D11_STENCIL_OP_KEEP);
		depthStencilDescription.BackFace.StencilDepthFailOp = backFace.stencilDepthFailOp.value_or(D3D11_STENCIL_OP_DECR);
		depthStencilDescription.BackFace.StencilPassOp = backFace.stencilPassOp.value_or(D3D11_STENCIL_OP_KEEP);
		depthStencilDescription.BackFace.StencilFunc = backFace.stencilFunc.value_or(D3D11_COMPARISON_ALWAYS);
	}

	if (FAILED(device->CreateDepthStencilState(&depthStencilDescription, m_depthStencilState.GetAddressOf()))) {
		m_status = false;
	}
}

DepthStencilState::DepthStencilState(ID3D11Device* device)
	: DepthStencilState(device, {}) { }

Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState::getDepthStencilState() const
{
	return m_depthStencilState;
}
