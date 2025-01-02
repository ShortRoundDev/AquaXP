#pragma once

struct CameraBuffer
{
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX inverseWorld;

};

namespace AquaXP
{
    template<typename T>
    class ICamera
    {
        static_assert(
            std::is_base_of<CameraBuffer, T>::value,
            "T must inherit from CameraBuffer"
        );
    public:
        ICamera(
            DirectX::XMMATRIX const& world,
            DirectX::XMMATRIX const& view,
            DirectX::XMMATRIX const& projection
        );

        T const& getCameraBuffer() const;

    protected:
        void setWorld(DirectX::XMMATRIX const& world);
        void setView(DirectX::XMMATRIX const& view);
        void setProjection(DirectX::XMMATRIX const& projection);

        T m_cameraBuffer;
    };
}