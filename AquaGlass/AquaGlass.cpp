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

int main()
{
    /* Initialize Window and DirectX infrastructure */
    Application app(800, 600, L"AquaGlass", false, false, false, true);
    auto camera = make_shared<NoclipCamera>(
        XMVectorSet(0.0f, 8.0f, -16.0f, 0),
        XMVectorSet(0, 0, 0, 0),
        800.0f, 600.0f,
        M_PI / 4.0f,
        0.1f, 1000.0f
    );
    app.pushCamera(camera);
    app.setMouseMode(Mouse::Mode::MODE_RELATIVE);

    /* Graphics object contains the DX11 context and device objects */
    auto& graphics = app.getGraphics();
    auto device = graphics.getDevice().Get();
    auto context = graphics.getContext().Get();

    /* Build the Vertex Input Layot with default data formats*/
    InputLayoutBuilder layoutBuilder;
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
    CBuffer<CameraBuffer> matrices(device, camera->getCameraBuffer());
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

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("Assets/Platform.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FindDegenerates);
    std::vector<UINT> indices;
    std::vector<Vertex> vertices;
    unique_ptr<Texture> texture;
    for (u32 i = 1; i < scene->mNumMeshes; i++)
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
            texture = make_unique<Texture>(graphics, "Assets/" + string(texturePath.C_Str()));
            texture->use(context);
        }
    }
    auto c = XMVectorSet(0, 0, 0, 0);
    auto max = XMVectorSet(100, 100, 100, 0);

    auto test = XMVectorSet(34, 34, 34, 0);
    auto size = XMVectorSet(25, 25, 25, 0);
    int value = 123;

    OctreeNode<int> node(AABB(c, max));
    node.tryInsert(make_pair(AABB(test, size), &value));

    std::set<int*> aabb;
    node.tryQuery(AABB(c, max), aabb);

    /* Simple textured triangle */
    Mesh<Vertex> mesh(
        device,
        vertices,
        indices
    );
    mesh.use(context);

    static f32 color[4] = { 0.6f, 0.6f, 1.0f, 1.0f };
    app.run(
        [&](Application* appl)
        {
            matrices.setData(camera->getCameraBuffer());
            matrices.bind(context, 0);
            //graphics->resetRenderTarget();
            graphics.getBackBuffer()->clear(graphics, color);
            graphics.getDepthBuffer()->clearDepth(graphics);

            mesh.useAndDraw(context);
            graphics.present();
        },
        [&](Application* appl, f32 dt)
        {
            /* Rotate matrix on the Y Axis */
            time += dt;
            camera->update(app, dt);
            //camera->setPos(XMVectorSet(0, 8.0f, -16.0f, 0));
            //camera->lookAt(XMVectorSet(0, 0, 0, 0));
            //modelTransform = XMMatrixRotationY(time / 100000000.0f * M_PI * 4.0); /* Time delta is in microseconds */

            /*model.setData({
                .model = XMMatrixTranspose(modelTransform)
            });
            model.bind(context, 1);*/
        }
    );
}
