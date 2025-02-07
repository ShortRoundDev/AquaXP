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

        void setPos(DirectX::XMVECTOR const& pos);

        virtual void update(Application& application) override;
    private:
        DirectX::XMVECTOR m_pos;
    };
}