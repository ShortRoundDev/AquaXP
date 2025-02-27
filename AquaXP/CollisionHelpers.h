#pragma once

namespace AquaXP
{
    DirectX::XMVECTOR ClosestPointOnTriangle(
        DirectX::XMVECTOR const& point,
        DirectX::XMVECTOR const& a,
        DirectX::XMVECTOR const& b,
        DirectX::XMVECTOR const& c,
        DirectX::XMVECTOR const& normal
    );

    DirectX::XMVECTOR ClosestPointOnLineSegment(
        DirectX::XMVECTOR const& point,
        DirectX::XMVECTOR const& a,
        DirectX::XMVECTOR const& b
    );

    DirectX::XMVECTOR CollideSphereTriangle(
        DirectX::XMVECTOR const& center, f32 radius,
        DirectX::XMVECTOR const& velocity,
        DirectX::XMVECTOR const& a,
        DirectX::XMVECTOR const& b,
        DirectX::XMVECTOR const& c
    );
}