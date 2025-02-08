#include "pch.h"
#include "NoclipCamera.h"
#include "Application.h"
#include "MathHelpers.h"

#include <iostream>

using namespace AquaXP;
using namespace DirectX;
using namespace std;

AquaXP::NoclipCamera::NoclipCamera(
    XMVECTOR const& pos,
    XMVECTOR const& lookAt,
    f32 width,
    f32 height,
    f32 fov,
    f32 nearZ,
    f32 farZ
) :
    AquaXP::ICameraTemplate<CameraBuffer>(
        ProjectionType::Perspective,
        pos,
        LookAtToQuaternion(pos, lookAt),
        XMVectorSet(0, 1, 0, 0),
        width,
        height,
        fov,
        nearZ,
        farZ
    ) { }

void AquaXP::NoclipCamera::update(Application& application, f32 dt)
{
    if (application.getKeyboard().IsKeyDown(Keyboard::Keys::W))
    {
        move(getLook() * 0.01f);
    }

    f32 yaw = static_cast<f32>(application.getMouse().x) * 0.01f,
        pitch = -static_cast<f32>(application.getMouse().y) * 0.01f;

    auto yawRot = XMQuaternionRotationAxis(m_up, yaw);
    m_rotation = XMQuaternionMultiply(m_rotation, yawRot);

    auto right = XMVector3Cross(getLook(), m_up);

    auto pitchQuat = XMQuaternionRotationAxis(right, pitch);
    m_rotation = XMQuaternionMultiply(m_rotation, pitchQuat);

    /*m_rotation = XMQuaternionMultiply(
        m_rotation,
        XMQuaternionRotationRollPitchYaw(pitch, yaw, 0)
    );*/

    ICameraTemplate::update(application, dt);
}