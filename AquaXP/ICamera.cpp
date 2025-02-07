#include "pch.h"
#include "ICamera.h"
#include "MathHelpers.h"

using namespace DirectX;
using namespace AquaXP;

template<typename T>
ICameraTemplate<T>::ICameraTemplate(
    XMMATRIX const& world,
    XMMATRIX const& view,
    XMMATRIX const& projection
) :
    T({
        .world = world,
        .view = view,
        .projection = projection,
        .invWorld = InverseTranspose(world)
    }),
    m_cameraBufferSize(sizeof(T)) { }

template<typename T>
void ICameraTemplate<T>::setWorld(XMMATRIX const& world)
{
    m_cameraBuffer.world = world;
}

template<typename T>
void ICameraTemplate<T>::setView(XMMATRIX const& view)
{
    m_cameraBuffer.view = view;
}

template<typename T>
void ICameraTemplate<T>::setProjection(XMMATRIX const& projection)
{
    m_cameraBuffer.projection = projection;
}

template<typename T>
void const* ICameraTemplate<T>::getCameraBuffer() const
{
    return &m_cameraBuffer;
}

template<typename T>
sz ICameraTemplate<T>::getCameraBufferSize() const
{
    return m_cameraBufferSize;
}
