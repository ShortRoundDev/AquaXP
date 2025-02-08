#include "pch.h"
#include "NoclipCamera.h"
#include "Application.h"
#include "MathHelpers.h"

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

void AquaXP::NoclipCamera::update(Application& application)
{
    static const XMVECTOR s_baseForward = XMVectorSet(0, 0, 1, 0);
    if (application.getKeyboard().GetState().IsKeyDown(Keyboard::Keys::W))
    {
        move(getLook() * 0.01f);
    }
    
    ICameraTemplate::update(application);
}