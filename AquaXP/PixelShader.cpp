#include "pch.h"
#include "PixelShader.h"
#include "ShaderHelper.h"

using namespace AquaXP;

AQUAXP_API void AquaXP::UsePixelShader(ID3D11DeviceContext* context, PixelShader const& ps)
{
	context->PSSetShader(ps.Get(), nullptr, 0);
}