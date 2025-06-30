#pragma once

#include "Mesh.h"

namespace AquaXP
{
	struct LightVolumeVertex
	{
		DirectX::XMFLOAT4 m_position;
	};
	std::shared_ptr<AquaXP::Mesh<LightVolumeVertex>> CreateSphere(ID3D11Device* device, u32 rings, u32 sectors);
}