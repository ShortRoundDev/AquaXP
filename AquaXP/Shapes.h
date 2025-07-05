#pragma once

#include "Mesh.h"

namespace AquaXP
{
	struct SimplePositionVertex
	{
		DirectX::XMFLOAT3 m_position;
	};

	struct ScreenQuadVertex
	{
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT2 m_texCoords;
	};

	AQUAXP_API std::shared_ptr<AquaXP::Mesh<SimplePositionVertex>> CreateSphere(ID3D11Device* device, u32 rings, u32 sectors);
	AQUAXP_API std::shared_ptr<AquaXP::Mesh<ScreenQuadVertex>> CreateFullScreenQuad(ID3D11Device* device);

}