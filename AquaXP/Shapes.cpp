#include "pch.h"
#include "Shapes.h"

using namespace AquaXP;
using namespace std;
using namespace DirectX;

Result<Mesh<SimplePositionVertex>> AquaXP::CreateSphere(ID3D11Device* device, u32 rings, u32 sectors)
{
	vector<SimplePositionVertex> vertices;
	vector<u32> indices;
	for (u32 ring = 0; ring <= rings; ring++)
	{
		f32 phi = static_cast<f32>(M_PI) * ring / rings;
		for (u32 sector = 0; sector <= sectors; sector++) {
			f32 theta = 2.0f * static_cast<f32>(M_PI) * sector / sectors;

			f32 x = sinf(phi) * cosf(theta),
				y = cosf(phi),
				z = sinf(phi) * sinf(theta);

			vertices.push_back({ .m_position = XMFLOAT3(x, y, z) });
		}
	}

	for (u32 ring = 0; ring < rings; ring++) {
		for (u32 sector = 0; sector < sectors; sector++) {
			u32 current = ring * (sectors + 1) + sector;
			u32 next = current + sectors + 1;

			if (ring != 0) {
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
			}

			if (ring != rings - 1) {
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}
	}
	return CreateMesh<SimplePositionVertex>(device, vertices, indices);
}

Result<AquaXP::Mesh<ScreenQuadVertex>> AquaXP::CreateFullScreenQuad(ID3D11Device* device)
{
	vector<ScreenQuadVertex> vertices = {
		{ .m_position = XMFLOAT3(-1.0f, +1.0f, +0.0f), .m_texCoords = XMFLOAT2(0, 0) },
		{ .m_position = XMFLOAT3(+1.0f, +1.0f, +0.0f), .m_texCoords = XMFLOAT2(1, 0) },
		{ .m_position = XMFLOAT3(+1.0f, -1.0f, +0.0f), .m_texCoords = XMFLOAT2(1, 1) },
		{ .m_position = XMFLOAT3(-1.0f, -1.0f, +0.0f), .m_texCoords = XMFLOAT2(0, 1) },
	};
	vector<UINT> indices = {
		0, 1, 2,
		0, 2, 3
	};
	return CreateMesh<ScreenQuadVertex>(device, vertices, indices);
}