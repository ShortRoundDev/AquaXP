#include "pch.h"
#include "NoclipCameraController.h"
#include "Application.h"
#include "MathHelpers.h"

#include <iostream>

using namespace AquaXP;
using namespace DirectX;
using namespace std;
using namespace std::numbers;

using Keys = Keyboard::Keys;

NoclipCameraController::NoclipCameraController(
    f32 pitch,
    f32 yaw
) : m_pitch(pitch),
    m_yaw(yaw) { }

void NoclipCameraController::update(Application& application, ICamera& camera, f32 dt)
{
    camera.move(
        XMVector3Rotate(application.getMove() * 0.5f, camera.getRotation())
    );

    auto look = application.getLook();

    f32 yaw = XMVectorGetX(look),
        pitch = XMVectorGetZ(look);

    m_yaw += yaw;
    m_pitch += pitch;

    f32 fpi = static_cast<f32>(pi);
    if (m_pitch >= fpi/2.0f)
    {
        m_pitch = fpi/2.0f - 0.01f;
    }
    else if (m_pitch <= -fpi/2.0f)
    {
        m_pitch = -fpi/2.0f + 0.01f;
    }
    camera.setRotation(XMQuaternionRotationRollPitchYaw(m_pitch, m_yaw, 0.0f));
}