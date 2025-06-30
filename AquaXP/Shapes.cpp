#include "pch.h"
#include "Shapes.h"

using namespace AquaXP;
using namespace std;
using namespace DirectX;

shared_ptr<Mesh<LightVolumeVertex>> CreateSphere(ID3D11Device* device, u32 rings, u32 sectors)
{
	vector<LightVolumeVertex> vertices;
	vector<u32> indices;
	for (u32 ring = 0; ring <= rings; ring++)
	{
		f32 phi = M_PI * ring / rings;
		for (u32 sector = 0; sector <= sectors; sector++) {
			f32 theta = 2.0f * M_PI * sector / sectors;

			f32 x = sinf(phi) * cosf(theta),
				y = cosf(phi),
				z = sinf(phi) * sinf(theta);

			vertices.push_back({ .m_position = XMFLOAT4(x, y, z, 0.0f) });
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

	return make_shared<Mesh<LightVolumeVertex>>(device, vertices, indices);
}