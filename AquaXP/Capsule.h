#pragma once
namespace AquaXP
{
    namespace Physics
    {
        class Capsule
        {
        public:
            Capsule(DirectX::XMVECTOR const& a, DirectX::XMVECTOR const& b, f32 radius) :
                m_a(a),
                m_b(b),
                m_radius(radius) { }

            inline DirectX::XMVECTOR const& getA() const;
            inline DirectX::XMVECTOR const& getB() const;
        private:
            DirectX::XMVECTOR m_a;
            DirectX::XMVECTOR m_b;

            f32 m_radius;
        };
    }
}