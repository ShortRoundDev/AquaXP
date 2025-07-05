#include "pch.h"
#include "CollisionHelpers.h"

using namespace DirectX;
using namespace AquaXP;
using namespace std;

constexpr static f32 EPSILON = 1e-6f;

#pragma warning( push )
#pragma warning( disable: 4715 )
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
#pragma warning( pop )

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
    i32 i = 0; /* axis YZ */
    f32 ax = fabsf(XMVectorGetX(normal)),
        ay = fabsf(XMVectorGetY(normal)),
        az = fabsf(XMVectorGetZ(normal));

    /* What cardinal plane is the normal closest to, for simple orthographic 2D projection.
     * We project to 2D because it's easier to check if a point is "left" of a 2D line segment
     * than to calculate 3 normals by cross product and check if the point is "in front" of the
     * faces formed by each edge of the triangle in 3D space. We need the plane by which to project
     * from because otherwise we could end up with very skinny triangles if we just hardcoded one
     * plane, and could get improper results due to lack of precision. */
    if (ay > ax)
    {
        i = 1; /* axis XZ */
    }
    if (az > ay && az > ax)
    {
        i = 2; /* axis XY */
    }

    /* Projections of each point from the chosen axis*/
    XMFLOAT2
        p = Project2D(point, i),
        a2 = Project2D(a, i),
        b2 = Project2D(b, i),
        c2 = Project2D(c, i);

    /* The sign of the area gives the winding order of the triangle. The winding order is not
     * important here, but it does determine which sign the signed area of the triangles formed by
     * p and each edge need to have to determine "left handedness". If the sign of the area of the
     * triangles by formed p and each edge are all the same ("correct") sign, then the point is
     * interior to the triangle because p is "left" of each edge. */
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
    /* Get the line segment as a vector */
    auto line = b - a;
    auto length = XMVectorGetX(XMVector3Length(line));

    /* A and B are equal, closest position will always be A or B, doesn't matter here */
    if (length == 0.0f)
    {
        return a;
    }
    /* Get the closest position between the queried point and the infinite line
     * #as a percentage of the length of the line SEGMENT# */
    f32 t = XMVectorGetX(XMVector3Dot(point - a, line)) / (length * length);

    /* Clamp the percentage of the length. If percent is < 0, it is closer to A. If it is > 1, it is closer to B*/
    t = Clampf(t, 0.0f, 1.0f);

    /* Reconstruct the point on the line segment by multiplying the line segment by the percentage of its length
     * then add A to get it in world coordinates */
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
    /* edge AB*/
    auto ab = b - a;
    /* edge AC */
    auto ac = c - a;

    /* The Signed distance between the queried point and the plane of the triangle */
    f32 signedDistance = XMVectorGetX(XMVector3Dot(point - a, normal));

    /* The closest position on the plane to the queried point*/
    auto planePoint = point - (normal * signedDistance);

    /* Use the signed area to determine if the point lies within the edges of the triangle.
     * Effectively, check if the point is "left" of each edge on the triangle, +/- some epsilon */
    if (pointIsInTriangle(planePoint, a, b, c, normal))
    {
        /* closest point is on the face of the triangle */
        return planePoint;
    }

    /* If we are here, that means the closest point is not on the triangle face, but is on an edge or vertex */

    /* */
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

XMVECTOR AquaXP::CollideSphereTriangle(
    XMVECTOR const& center,
    f32 radius,
    XMVECTOR const& velocity,
    XMVECTOR const& a,
    XMVECTOR const& b,
    XMVECTOR const& c,
    XMVECTOR const& normal
)
{
    /* Calculate where the sphere WANTS to be*/
    auto nextFrame = XMVectorAdd(center, velocity);

    /* Get the closest point on the triangle to where the sphere WANTS to be*/
    auto closestPoint = ClosestPointOnTriangle(
        nextFrame,
        a, b, c,
        normal
    );
    /* Vector between where the sphere wants to be and the closest point on the triangle*/
    auto toClosestPoint = XMVectorSubtract(closestPoint, nextFrame);
    /* (squred) Distance between that where the sphere wants to be and the closest point on the triangle*/
    auto distanceSq = XMVectorGetX(XMVector3LengthSq(toClosestPoint));
    /* If the distance is greater than the radius of the sphere, there is no collision */
    if (distanceSq >= (radius * radius))
    {
        return nextFrame;
    }
    /* Actual distance, not squared */
    auto distance = sqrtf(distanceSq);

    /* How far past the plane of the triangle is the point where the sphere wants to be */
    f32 depth = distance < EPSILON
        ? radius /* Collision is RIGHT on the edge of the sphere */
        : radius - distance;

    /* What direction to resolve the sphere towards (i.e, how far out do we need to
     * push where the sphere wants to be so that it is outside of the triangle */
    auto direction = normal;
    if (XMVectorGetX(XMVector3Dot(toClosestPoint, normal)) > 0)
    {
        /* Make sure it pushes in the right direction*/
        direction = XMVectorScale(direction, -1.0f);
    }

    /* Push where the sphere wants to be outside of the triangle by the penetration depth */
    return XMVectorAdd(nextFrame, XMVectorScale(direction, depth));
}