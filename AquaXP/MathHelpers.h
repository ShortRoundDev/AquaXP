#pragma once

namespace AquaXP
{
    inline void InverseTranspose(DirectX::XMMATRIX const& world, DirectX::XMMATRIX& invWorld)
    {
        CopyMemory(&invWorld, &world, sizeof(DirectX::XMMATRIX));
        invWorld.r[3] = DirectX::XMVectorSet(0, 0, 0, 1.0f);
        DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant(invWorld);
        invWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&determinant, invWorld));
    }

    inline DirectX::XMMATRIX InverseTranspose(DirectX::XMMATRIX const& world)
    {
        DirectX::XMMATRIX invWorld;
        InverseTranspose(world, invWorld);
        return invWorld;
    }

    inline DirectX::XMVECTOR LookAtToQuaternion(DirectX::XMVECTOR const& pos, DirectX::XMVECTOR const& lookAt)
    {
        static const DirectX::XMVECTOR s_baseForward = DirectX::XMVectorSet(0, 0, 1, 0);
        DirectX::XMVECTOR look, q;
        look = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(lookAt, pos));
        q = DirectX::XMVector3Cross(s_baseForward, look);
        return DirectX::XMVectorSetW(q, 1 + DirectX::XMVectorGetX(DirectX::XMVector3Dot(s_baseForward, look)));

    }

    inline f32 Clampf(f32 t, f32 min, f32 max)
    {
        return fmin(max, fmax(t, min));
    }
};
