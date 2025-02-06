#pragma once

#include <array>

namespace AquaXP
{
    class AABB
    {
    public:
        AABB(DirectX::XMVECTOR const& position, DirectX::XMVECTOR const& halfWidth) :
            m_position(position),
            m_halfWidth(halfWidth)
        {
        }

        DirectX::XMVECTOR const& getPosition() const
        {
            return m_position;
        }

        void setPosition(DirectX::XMVECTOR const& position)
        {
            m_position = position;
        }

        void translate(DirectX::XMVECTOR const& diff)
        {
            m_position = DirectX::XMVectorAdd(m_position, diff);
        }

        DirectX::XMVECTOR getMax() const
        {
            return DirectX::XMVectorAdd(m_position, m_halfWidth);
        }

        DirectX::XMVECTOR getMin() const
        {
            return DirectX::XMVectorSubtract(m_position, m_halfWidth);
        }

        DirectX::XMVECTOR const& getHalfWidth() const
        {
            return m_halfWidth;
        }

        bool overlap(AABB const& b) const
        {
            auto ac = getPosition();
            auto bc = b.getPosition();
            auto ar = getHalfWidth();
            auto br = b.getHalfWidth();

            auto cDiff = DirectX::XMVectorAbs(DirectX::XMVectorSubtract(ac, bc));
            auto rSum = DirectX::XMVectorAdd(ar, br);
            auto overlaps = DirectX::XMVectorSetW(DirectX::XMVectorLess(cDiff, rSum), 0);
            /*auto tst = x == 0xFFFFFFFF;
            return DirectX::XMVectorGetIntX(overlaps) == 0xFFFFFFFF &&
                DirectX::XMVectorGetIntY(overlaps) == 0xFFFFFFFF &&
                DirectX::XMVectorGetIntZ(overlaps) == 0xFFFFFFFF;*/
            auto intVec = DirectX::XMVectorSetInt(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
            return DirectX::XMVector3EqualInt(overlaps, intVec);
        }

    private:
        DirectX::XMVECTOR m_position;
        DirectX::XMVECTOR m_halfWidth;
    };

    void SubdivideAABB(AABB const& in, std::array<AABB, 8>& out)
    {
        auto pos = in.getPosition();
        auto half = in.getHalfWidth();
        auto quarter = DirectX::XMVectorScale(half, 0.5f);
        out[0] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(-1, -1, -1, 1))), quarter);
        out[1] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(+1, -1, -1, 1))), quarter);
        out[2] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(-1, +1, -1, 1))), quarter);
        out[3] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(+1, +1, -1, 1))), quarter);
        out[4] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(-1, -1, +1, 1))), quarter);
        out[5] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(+1, -1, +1, 1))), quarter);
        out[6] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(-1, +1, +1, 1))), quarter);
        out[7] = AABB(DirectX::XMVectorAdd(pos, DirectX::XMVectorMultiply(half, DirectX::XMVectorSet(+1, +1, +1, 1))), quarter);
    }
}