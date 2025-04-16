#pragma once
#include "CubeMap.h"

namespace AquaXP
{
    class PointLight
    {
    public:
        PointLight(
            DirectX::XMVECTOR const& position
        );

    private:
        std::unique_ptr<CubeMap> m_shadowMap;
    };
}