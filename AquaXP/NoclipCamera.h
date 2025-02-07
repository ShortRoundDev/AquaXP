#pragma once
#include "ICamera.h"

namespace AquaXP
{
    class NoclipCamera : public ICameraTemplate<CameraBuffer>
    {
    public:
        NoclipCamera(
            DirectX::XMVECTOR const& pos,
            DirectX::XMVECTOR const& lookAt
        );

        virtual void update(Application& application) override;
    };
}