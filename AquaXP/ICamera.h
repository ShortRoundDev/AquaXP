#pragma once

namespace AquaXP
{
    struct CameraBuffer
    {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
        DirectX::XMVECTOR pos;
    };

    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    class Application;

    class ICamera
    {
    public:
        virtual CameraBuffer const* getCameraBuffer() const = 0;
        virtual sz getCameraBufferSize() const = 0;

        virtual void update(Application& application) = 0;

        virtual void setPos(DirectX::XMVECTOR const& pos) = 0;
        virtual DirectX::XMVECTOR const& getPos() const = 0;

        virtual void setRotation(DirectX::XMVECTOR const& quaternion) = 0;
        virtual DirectX::XMVECTOR const& getRotation() const = 0;

        virtual void setProjectionType(ProjectionType projectionType) = 0;
        virtual ProjectionType getProjectionType() const = 0;

        virtual void lookAt(DirectX::XMVECTOR const& pos) = 0;
        virtual DirectX::XMVECTOR const& getLook() const = 0;

        virtual void move(DirectX::XMVECTOR const& diff) = 0;

        virtual void setWidth(f32 width) const = 0;
        virtual void setHeight(f32 height) const = 0;

        virtual f32 getAspectRation() const = 0;

        virtual void setFov(f32 fov) const = 0;
        virtual f32 getFov() const = 0;

        virtual void setNear(f32 nearZ) const = 0;
        virtual f32 getNear() const = 0;

        virtual void setFar(f32 farZ) const = 0;
        virtual f32 getFar() const = 0;
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
            ProjectionType projectionType,
            DirectX::XMVECTOR const& pos,
            DirectX::XMVECTOR const& rotation,
            f32 width,
            f32 height,
            f32 nearZ,
            f32 farZ
        ) :


        virtual T const* getCameraBuffer() const override
        {
            return m_cameraBuffer;
        }

        virtual sz getCameraBufferSize() const override
        {
            return sizeof(T);
        }

        virtual void setPos(DirectX::XMVECTOR const& pos) override
        {
            m_pos = pos;
        }

        virtual DirectX::XMVECTOR const& getPos() const override
        {
            return m_pos;
        }

        virtual void setRotation(DirectX::XMVECTOR const& quaternion) override
        {
            m_rotation = quaternion;
        }

        virtual DirectX::XMVECTOR const& getRotation() const override
        {
            return m_rotation;
        }

        virtual void setProjectionType(ProjectionType projectionType) override
        {
            m_projectionType = projectionType;
        }

        virtual ProjectionType getProjectionType() const override
        {
            return m_projectionType;
        }

        virtual void lookAt(DirectX::XMVECTOR const& pos) override
        {
            static const s_baseForward = DirectX::XMVectorSet(0, 0, 1, 0);
            DirectX::XMVECTOR look, q, a;
            look = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(m_pos, pos));
            q = DirectX::XMVector3Cross(s_baseForward, look);
            q = a;
            return DirectX::XMVectorSetW(q, 1 + DirectX::XMVectorGetX(DirectX::XMVector3Dot(s_baseForward, look)));
        }

        virtual DirectX::XMVECTOR const& getLook() const override
        {
            // rotate forward vec by quaternion
        }

        virtual void move(DirectX::XMVECTOR const& diff) override;

        virtual void setWidth(f32 width) const override;
        virtual void setHeight(f32 height) const override;

        virtual f32 getAspectRation() const override;

        virtual void setFov(f32 fov) const override;
        virtual f32 getFov() const override;

        virtual void setNear(f32 nearZ) const override;
        virtual f32 getNear() const override;

        virtual void setFar(f32 farZ) const override;
        virtual f32 getFar() const override;

    protected:
        T m_cameraBuffer;

        ProjectionType m_projectionType;
        DirectX::XMVECTOR const& m_pos;
        DirectX::XMVECTOR const& m_rotation;
        DirectX::XMVECTOR const& m_up;
        f32 m_width;
        f32 m_height;
        f32 m_nearZ;
        f32 m_farZ;

    };
}