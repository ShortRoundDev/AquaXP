#include "pch.h"
#include "ICamera.h"
#include "MathHelpers.h"

using namespace DirectX;
using namespace AquaXP;

template<typename T>
ICamera<T>::ICamera(
    XMMATRIX const& world,
    XMMATRIX const& view,
    XMMATRIX const& projection
) :
    T({
        .world = world,
        .view = view,
        .projection = projection,
        .invWorld = InverseTranspose(world)
    })
{

}

template<typename T>
void ICamera<T>::setWorld(XMMATRIX const& world)
{
    m_cameraBuffer.world = world;
}

template<typename T>
void ICamera<T>::setView(XMMATRIX const& view)
{
    m_cameraBuffer.view = view;
}

template<typename T>
void ICamera<T>::setProjection(XMMATRIX const& projection)
{
    m_cameraBuffer.projection = projection;
}

template<typename T>
T const& ICamera<T>::getCameraBuffer() const
{
    return m_cameraBuffer;
}
