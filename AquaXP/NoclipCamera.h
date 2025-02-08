#pragma once
#include "ICamera.h"

namespace AquaXP
{
    class NoclipCamera : public ICameraTemplate<CameraBuffer>
    {
    public:
        AQUAXP_API NoclipCamera(
            DirectX::XMVECTOR const& pos,
            DirectX::XMVECTOR const& lookAt,
            f32 width,
            f32 height,
            f32 fov,
            f32 nearZ,
            f32 farZ
        );

        AQUAXP_API virtual void update(Application& application) override;
    };
}