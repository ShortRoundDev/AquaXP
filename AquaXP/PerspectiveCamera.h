#pragma once
#include "ICamera.h"

namespace AquaXP
{
    template<typename T = CameraBuffer>
    class PerspectiveCamera :
        public ICamera<T>
    {
    public:
        PerspectiveCamera(
            DirectX::XMVECTOR pos,
            DirectX::XMVECTOR look,
            f32 fov,
            f32 width,
            f32 height
        );

        PerspectiveCamera(
            DirectX::XMVECTOR pos,
            DirectX::XMVECTOR look,
            DirectX::XMVECTOR up,
            DirectX::XMVECTOR rotation,
            f32 fov,
            f32 width,
            f32 height
        );

        void setPos(DirectX::XMVECTOR const& pos);
        void setLook(DirectX::XMVECTOR const& look);
        void setRotation(DirectX::XMVECTOR const& rotation);
        void setUp(DirectX::XMVECTOR const& up);

    protected:
        DirectX::XMVECTOR m_pos;
        DirectX::XMVECTOR m_look;
        DirectX::XMVECTOR m_up;
        DirectX::XMVECTOR m_rotate;
    };
}