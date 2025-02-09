#pragma once
#include "ICamera.h"

namespace AquaXP
{
    class NoclipCameraController : public ICameraController
    {
    public:
        AQUAXP_API NoclipCameraController(
            f32 pitch = 0.0f,
            f32 yaw = 0.0f
        );

        AQUAXP_API virtual void update(Application& application, ICamera& camera, f32 dt) override;

    private:
        f32 m_pitch;
        f32 m_yaw;
    };
}