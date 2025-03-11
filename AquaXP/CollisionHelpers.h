#pragma once

namespace AquaXP
{
    AQUAXP_API DirectX::XMVECTOR ClosestPointOnTriangle(
        DirectX::XMVECTOR const& point,
        DirectX::XMVECTOR const& a,
        DirectX::XMVECTOR const& b,
        DirectX::XMVECTOR const& c,
        DirectX::XMVECTOR const& normal
    );

    AQUAXP_API DirectX::XMVECTOR ClosestPointOnLineSegment(
        DirectX::XMVECTOR const& point,
        DirectX::XMVECTOR const& a,
        DirectX::XMVECTOR const& b
    );

    AQUAXP_API DirectX::XMVECTOR CollideSphereTriangle(
        DirectX::XMVECTOR const& center,
        f32 radius,
        DirectX::XMVECTOR const& velocity,
        DirectX::XMVECTOR const& a,
        DirectX::XMVECTOR const& b,
        DirectX::XMVECTOR const& c,
        DirectX::XMVECTOR const& normal
    );
}