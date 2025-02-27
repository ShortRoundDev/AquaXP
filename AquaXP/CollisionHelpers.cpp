#include "pch.h"
#include "CollisionHelpers.h"

using namespace DirectX;
using namespace AquaXP;
using namespace std;

constexpr static f32 EPSILON = 1e-6f;

XMFLOAT2 Project2D(XMVECTOR const& v, i32 axis)
{
    switch (axis)
    {
    case 0:
        return XMFLOAT2(XMVectorGetY(v), XMVectorGetZ(v));
    case 1:
        return XMFLOAT2(XMVectorGetX(v), XMVectorGetZ(v));
    case 2:
        return XMFLOAT2(XMVectorGetX(v), XMVectorGetY(v));
    }
}

f32 SignedArea(XMFLOAT2 const& a, XMFLOAT2 const& b, XMFLOAT2 const& p)
{
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

bool pointIsInTriangle(
    XMVECTOR const& point,
    XMVECTOR const& a,
    XMVECTOR const& b,
    XMVECTOR const& c,
    XMVECTOR const& normal
)
{
    i32 i = 0;
    f32 ax = fabsf(XMVectorGetX(normal)),
        ay = fabsf(XMVectorGetY(normal)),
        az = fabsf(XMVectorGetZ(normal));

    /* What cardinal plane is the normal closest to, for simple 2D projection */
    if (ay > ax)
    {
        i = 1;
    }
    if (az > ay && az > ax)
    {
        i = 2;
    }

    XMFLOAT2
        p = Project2D(point, i),
        a2 = Project2D(a, i),
        b2 = Project2D(b, i),
        c2 = Project2D(c, i);

    f32 area = SignedArea(a2, b2, c2);
    if (area < 0)
    {
        return
            SignedArea(a2, b2, p) <= EPSILON &&
            SignedArea(b2, c2, p) <= EPSILON &&
            SignedArea(c2, a2, p) <= EPSILON;
    }
    else
    {
        return
            SignedArea(a2, b2, p) >= -EPSILON &&
            SignedArea(b2, c2, p) >= -EPSILON &&
            SignedArea(c2, a2, p) >= -EPSILON;
    }
}

XMVECTOR AquaXP::ClosestPointOnLineSegment(
    XMVECTOR const& point,
    XMVECTOR const& a,
    XMVECTOR const& b
)
{
    auto line = b - a;
    auto length = XMVectorGetX(XMVector3Length(line));
    if (length == 0.0f)
    {
        return a;
    }
    f32 t = XMVectorGetX(XMVector3Dot(point - a, line)) / (length * length);

    t = Clampf(t, 0.0f, 1.0f);

    return a + (line * t);
}

XMVECTOR AquaXP::ClosestPointOnTriangle(
    XMVECTOR const& point,
    XMVECTOR const& a,
    XMVECTOR const& b,
    XMVECTOR const& c,
    XMVECTOR const& normal
)
{
    auto ab = b - a;
    auto ac = c - a;

    f32 signedDistance = XMVectorGetX(XMVector3Dot(point - a, normal));
    auto planePoint = point - (normal * signedDistance);

    if (pointIsInTriangle(planePoint, a, b, c, normal))
    {
        return planePoint;
    }

    auto closestOnAB = ClosestPointOnLineSegment(point, a, b);
    auto closestOnBC = ClosestPointOnLineSegment(point, b, c);
    auto closestOnCA = ClosestPointOnLineSegment(point, c, a);

    f32 distanceAB = XMVectorGetX(XMVector3LengthSq(point - closestOnAB));
    f32 distanceBC = XMVectorGetX(XMVector3LengthSq(point - closestOnBC));
    f32 distanceCA = XMVectorGetX(XMVector3LengthSq(point - closestOnCA));

    if (distanceAB <= distanceBC && distanceAB <= distanceCA)
    {
        return closestOnAB;
    }
    if (distanceBC <= distanceCA)
    {
        return closestOnBC;
    }
    return closestOnCA;
}
