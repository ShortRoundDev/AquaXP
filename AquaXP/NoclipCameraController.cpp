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
    if (application.getKeyboard().IsKeyDown(Keys::W))
    {
        camera.move(camera.getLook() * 0.1f);
    }

    f32 yaw = static_cast<f32>(application.getMouse().x) * 0.01f,
        pitch = static_cast<f32>(application.getMouse().y) * 0.01f;

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