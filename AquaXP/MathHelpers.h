#pragma once

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