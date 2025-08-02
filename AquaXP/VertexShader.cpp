#include "pch.h"
#include "VertexShader.h"

using namespace std;
using namespace AquaXP;
using namespace Microsoft::WRL;

VertexShader::VertexShader(
	ComPtr<ID3D11VertexShader> shader,
	ComPtr<ID3D11InputLayout> inputLayout
) : m_shader(shader),
	m_inputLayout(inputLayout)
{ }

void VertexShader::use(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(getInputLayout().Get());
	context->VSSetShader(getShader().Get(), nullptr, 0);
}

ComPtr<ID3D11VertexShader> VertexShader::getShader() const
{
	return m_shader;
}

ComPtr<ID3D11InputLayout> VertexShader::getInputLayout() const
{
	return m_inputLayout;
}