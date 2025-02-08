#pragma once

#include "MathHelpers.h"

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
        virtual void updateCameraBuffer() = 0;
        virtual CameraBuffer const& getCameraBuffer() const = 0;
        virtual sz getCameraBufferSize() const = 0;

        virtual void update(Application& application, f32 dt) = 0;

        virtual void setPos(DirectX::XMVECTOR const& pos) = 0;
        virtual DirectX::XMVECTOR const& getPos() const = 0;

        virtual void setRotation(DirectX::XMVECTOR const& quaternion) = 0;
        virtual DirectX::XMVECTOR const& getRotation() const = 0;

        virtual void setProjectionType(ProjectionType projectionType) = 0;
        virtual ProjectionType getProjectionType() const = 0;

        virtual void lookAt(DirectX::XMVECTOR const& pos) = 0;
        virtual DirectX::XMVECTOR getLook() const = 0;

        virtual void setUp(DirectX::XMVECTOR const& up) = 0;
        virtual DirectX::XMVECTOR const& getUp() const = 0;

        virtual void move(DirectX::XMVECTOR const& diff) = 0;

        virtual void setWidth(f32 width) = 0;
        virtual f32 getWidth() const = 0;

        virtual void setHeight(f32 height) = 0;
        virtual f32 getHeight() const = 0;

        virtual f32 getAspectRatio() const = 0;

        virtual void setFov(f32 fov) = 0;
        virtual f32 getFov() const = 0;

        virtual void setNear(f32 nearZ) = 0;
        virtual f32 getNear() const = 0;

        virtual void setFar(f32 farZ) = 0;
        virtual f32 getFar() const = 0;
    };

    template<typename T = CameraBuffer>
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
            DirectX::XMVECTOR const& up,
            f32 width,
            f32 height,
            f32 fov,
            f32 nearZ,
            f32 farZ
        ) :
            m_projectionType(projectionType),
            m_pos(pos),
            m_rotation(rotation),
            m_up(up),
            m_width(width),
            m_height(height),
            m_fov(fov),
            m_nearZ(nearZ),
            m_farZ(farZ),
            m_cameraBuffer() { }


        virtual void updateCameraBuffer() override
        {
            m_cameraBuffer.view = DirectX::XMMatrixTranspose(
                DirectX::XMMatrixLookAtLH(
                    m_pos,
                    DirectX::XMVectorAdd(m_pos, getLook()),
                    m_up
                )
            );
            switch (m_projectionType)
            {
            case ProjectionType::Orthographic:
            {
                m_cameraBuffer.projection = DirectX::XMMatrixTranspose(
                    DirectX::XMMatrixOrthographicLH(
                        m_width, m_height, m_nearZ, m_farZ
                    )
                );
                break;
            }
            case ProjectionType::Perspective:
            {
                m_cameraBuffer.projection = DirectX::XMMatrixTranspose(
                    DirectX::XMMatrixPerspectiveFovLH(
                        m_fov, m_width / m_height, m_nearZ, m_farZ
                    )
                );
                break;
            }
            }
            m_cameraBuffer.pos = m_pos;
        }

        virtual void update(Application& application, f32 dt) override
        {
            updateCameraBuffer();
        }

        virtual CameraBuffer const& getCameraBuffer() const
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
            m_rotation = LookAtToQuaternion(m_pos, pos);
        }

        virtual DirectX::XMVECTOR getLook() const override
        {
            static const DirectX::XMVECTOR s_baseForward = DirectX::XMVectorSet(0, 0, 1, 0);
            return DirectX::XMVector3Rotate(s_baseForward, m_rotation);
        }

        virtual void move(DirectX::XMVECTOR const& diff) override
        {
            m_pos = DirectX::XMVectorAdd(m_pos, diff);
        }

        virtual void setUp(DirectX::XMVECTOR const& up) override
        {
            m_up = up;
        }

        virtual DirectX::XMVECTOR const& getUp() const override
        {
            return m_up;
        }

        virtual void setWidth(f32 width) override
        {
            m_width = width;
        }

        virtual f32 getWidth() const override
        {
            return m_width;
        }

        virtual void setHeight(f32 height) override
        {
            m_height = height;
        }

        virtual f32 getHeight() const override
        {
            return m_height;
        }

        virtual f32 getAspectRatio() const override
        {
            return m_width / m_height;
        }

        virtual void setFov(f32 fov) override
        {
            m_fov = fov;
        }

        virtual f32 getFov() const override
        {
            return m_fov;
        }

        virtual void setNear(f32 nearZ) override
        {
            m_nearZ = nearZ;
        }

        virtual f32 getNear() const override
        {
            return m_nearZ;
        }

        virtual void setFar(f32 farZ) override
        {
            m_farZ = farZ;
        }

        virtual f32 getFar() const override
        {
            return m_farZ;
        }

    protected:
        T m_cameraBuffer;

        ProjectionType m_projectionType;
        DirectX::XMVECTOR m_pos;
        DirectX::XMVECTOR m_rotation;
        DirectX::XMVECTOR m_up;
        f32 m_fov;
        f32 m_width;
        f32 m_height;
        f32 m_nearZ;
        f32 m_farZ;

    };
}