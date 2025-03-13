// AquaGlass.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "AquaXP.h"
#include "DirectXMath.h"

using namespace AquaXP;
using namespace DirectX;

constexpr f32 M_PI = 3.14159f;

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
    XMFLOAT3 normal;
    XMFLOAT2 uv;
};

struct Model
{
    XMMATRIX model;
};

optional<pair<shared_ptr<Mesh<Vertex>>, shared_ptr<Texture>>> loadMesh(Graphics const& graphics, ID3D11Device *device, ID3D11DeviceContext *context, string const& name)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FindDegenerates);
    if (scene == nullptr)
    {
        return nullopt;
    }
    std::vector<UINT> indices;
    std::vector<Vertex> vertices;
    shared_ptr<Texture> texture;
    for (u32 i = 0; i < scene->mNumMeshes; i++)
    {
        auto mesh = scene->mMeshes[i];
        for (u32 j = 0; j < mesh->mNumVertices; j++)
        {
            auto pos = mesh->mVertices[j];
            auto tex = mesh->mTextureCoords[0][j];
            auto normal = mesh->mNormals[j];

            vertices.push_back({
                .pos = XMFLOAT3(pos.x, pos.y, pos.z),
                .color = XMFLOAT4(0, 0, 0, 0),
                .normal = XMFLOAT3(normal.x, normal.y, normal.z),
                .uv = XMFLOAT2(tex.x, tex.y),
                });
        }
        for (u32 j = 0; j < mesh->mNumFaces; j++)
        {
            auto face = mesh->mFaces[j];
            for (u32 k = 0; k < face.mNumIndices; k++)
            {
                indices.push_back(face.mIndices[k]);
            }
        }
        auto material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texturePath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
        {
            texture = make_shared<Texture>(graphics, "Assets/" + string(texturePath.C_Str()));
        }
    }
    return make_pair(
        make_shared<Mesh<Vertex>>(
            device,
            vertices,
            indices
        ),
        texture
    );
}

template<typename T>
using SomeAlloc = std::allocator<T>;

template<typename T>
using MyVector = std::vector<T, SomeAlloc<T>>;

int main()
{
    /* Initialize Window and DirectX infrastructure */
    Application app(800, 600, L"AquaGlass", false, false, false, true);

    /* Bind keys to actions */
    app.tryBindAction(DefaultActions::Forward, Keyboard::Keys::W);
    app.tryBindAction(DefaultActions::Back, Keyboard::Keys::S);
    app.tryBindAction(DefaultActions::Left, Keyboard::Keys::A);
    app.tryBindAction(DefaultActions::Right, Keyboard::Keys::D);

    /* Camera context joins the rendering parameters of a camera (its matrices)
     * with the control logic of a camera */
    CameraContext cameraContext(
        make_shared<NoclipCameraController>(),  /* Simply WAS camera controller */
        make_shared<ICameraTemplate<CameraBuffer>>(  /* Simple perspective projection */
            ProjectionType::Perspective,
            XMVectorSet(0.0f, 8.0f, -16.0f, 0),
            LookAtToQuaternion(
                XMVectorSet(0.0f, 8.0f, -16.0f, 0),
                XMVectorSet(0.0f, 8.0f, -15.0f, 0)
            ),
            XMVectorSet(0, 1, 0, 0),
            800.0f, 600.0f,
            M_PI / 4.0f,
            0.1f, 1000.0f
        )
    );

    /* Camera Contexts are a stack so you can swap between contexts as needed */
    app.pushCameraContext(cameraContext);

    /* Mouse look on */
    app.setMouseMode(Mouse::Mode::MODE_RELATIVE);

    /* Graphics object contains the DX11 context and device objects */
    auto& graphics = app.getGraphics();
    auto device = graphics.getDevice().Get();
    auto context = graphics.getContext().Get();

    /* Build the Vertex Input Layot with default data formats*/
    InputLayoutBuilder<MyVector> layoutBuilder;
    layoutBuilder
        .addPosition()
        .addColor()
        .addNormal()
        .addTexCoord();

    /* Load Shaders from precompiled CSO files. Vertex Shaders require the input layout */
    VertexShader vs(device, L"WorldVertex.cso", layoutBuilder.build());
    vs.use(context);

    PixelShader ps(device, L"WorldPixel.cso");
    ps.use(context);

    /* Create a sampler with default linear filtering settings */
    Sampler sampler(device);
    sampler.use(context);

    /* Create a CBuffer with necessary 3D projection projection and view matrices */
    CBuffer<CameraBuffer> matrices(device, cameraContext.m_camera->getCameraBuffer());
    matrices.bind(context, 0);

    /* Initialize model transform to identity matrix */
    XMMATRIX modelTransform = XMMatrixIdentity();
    /* Transformation matrix for the model being displayed */
    CBuffer<Model> model(device, {
        .model = XMMatrixTranspose(modelTransform)
    });
    model.bind(context, 1);
    /* Time accumulator for rotation */
    f32 time = 0.0f;

    /* Ball with a cow texture on it */
    auto cowBall = loadMesh(graphics, device, context, "Assets/cowball.obj");

    /* Simple textured triangle */
    auto wall = loadMesh(graphics, device, context, "Assets/wall.obj");

    /* Failed to load for some reason */
    if (!cowBall.has_value() || !wall.has_value())
    {
        return -1;
    }

    /* Background color */
    static f32 color[4] = { 0.6f, 0.6f, 1.0f, 1.0f };

    /* Movement vector and position for the cowball*/
    auto move = XMVectorSet(0, 0, -0.02f, 0.0f);
    auto position = XMVectorSet(0.0f, 0, 6.0f, 0.0f);

    /* Hard coded vertices for the triangle, to be used with physics collisions. */
    auto
        v0 = XMVectorSet(3.00000000f, -2.12132001f, -0.121320002f, 0.0f),
        v1 = XMVectorSet(-3.00000000f, -2.12132001f, -0.121320002, 0.0f),
        v2 = XMVectorSet(0.00000000f, 2.12132001f, 4.12132120f, 0.0f);

    /* Calculated normal for the triangle */
    auto n = XMVector3Normalize(
        XMVector3Cross(v1 - v0, v2 - v0)
    );

    app.run(
        [&](Application* appl)
        {
            /* Set Camera context on the "matrices" cbuffer*/
            matrices.setData(cameraContext.m_camera->getCameraBuffer());
            matrices.bind(context, 0);

            graphics.getBackBuffer()->clear(graphics, color);
            graphics.getDepthBuffer()->clearDepth(graphics);

            /* Set model transform for the ball */
            model.setData({
                .model = XMMatrixTranspose(XMMatrixTranslation(
                    XMVectorGetX(position),
                    XMVectorGetY(position),
                    XMVectorGetZ(position)
                ))
            });
            model.bind(context, 1);

            /* item2 is the texture loaded from the obj */
            get<1>(cowBall.value())->use(context);
            /* item1 is the mesh */
            get<0>(cowBall.value())->useAndDraw(context);

            /* No transform for the wall*/
            model.setData({
                .model = XMMatrixTranspose(XMMatrixIdentity())
            });
            model.bind(context, 1);

            get<1>(wall.value())->use(context);
            get<0>(wall.value())->useAndDraw(context);

            graphics.present();
        },
        [&](Application* appl, f32 dt)
        {
            time += dt;

            /* Move the sphere against the wall */
            position = CollideSphereTriangle(
                position,
                1.0f,
                move,
                v0,
                v1,
                v2,
                n
            );
        }
    );
}