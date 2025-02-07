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
    class Application;

    class ICamera
    {
    public:
        virtual void const* getCameraBuffer() const = 0;
        virtual sz getCameraBufferSize() const = 0;

        virtual void update(Application& application) = 0;

        virtual void setWorld(DirectX::XMMATRIX const& world) = 0;
        virtual void setView(DirectX::XMMATRIX const& view) = 0;
        virtual void setProjection(DirectX::XMMATRIX const& projection) = 0;
    };

    template<typename T>
    class ICameraTemplate : public ICamera
    {
        static_assert(
            std::is_base_of<CameraBuffer, T>::value,
            "T must inherit from CameraBuffer"
        );
    public:
        ICameraTemplate(
            DirectX::XMMATRIX const& world,
            DirectX::XMMATRIX const& view,
            DirectX::XMMATRIX const& projection
        );

        virtual sz getCameraBufferSize() const override;
        virtual void const* getCameraBuffer() const override;
        void setWorld(DirectX::XMMATRIX const& world) override;
        void setView(DirectX::XMMATRIX const& view) override;
        void setProjection(DirectX::XMMATRIX const& projection) override;

    protected:
        T m_cameraBuffer;
        sz m_cameraBufferSize;
    };
}