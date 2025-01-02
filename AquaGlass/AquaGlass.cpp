// AquaGlass.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>

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

struct Matrices
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

struct Model
{
    XMMATRIX model;
};

int main()
{
    /* Initialize Window and DirectX infrastructure */
    Application app(800, 600, L"AquaGlass", false, false, false, true);

    /* Graphics object contains the DX11 context and device objects */
    auto graphics = app.getGraphics();
    auto device = graphics->getDevice().Get();
    auto context = graphics->getContext().Get();

    /* Build the Vertex Input Layot with default data formats*/
    InputLayoutBuilder layoutBuilder;
    layoutBuilder
        .addPosition()
        .addColor()
        .addNormal()
        .addTexCoord();

    /* Load Shaders from precompiled CSO files. Vertex Shaders require the input layout */
    VertexShader vs(device, L"WorldVertex.cso", layoutBuilder);
    vs.use(context);
    
    PixelShader ps(device, L"WorldPixel.cso");
    ps.use(context);
    
    /* Create a sampler with default linear filtering settings */
    Sampler sampler(device);
    sampler.use(context);

    /* Load an image, bound to an SRV by default */
    Texture texture(graphics, L"Assets/Box.png");
    Texture shadowMap(graphics, 800.0f, 600.0f, { .Count = 1, .Quality = 0 }, D3D11_BIND_DEPTH_STENCIL);
    texture.use(context);

    /* Create a CBuffer with necessary 3D projection projection and view matrices */
    CBuffer<Matrices> matrices(device, {
        .world = XMMatrixTranspose(XMMatrixIdentity()), /* Matrices need to be transposed because the GPU uses column-major representation */
        .view = XMMatrixTranspose(XMMatrixLookAtLH(
            XMVectorSet(0, 0, -3.0f, 0), /* Camera is pulled back 3 units*/
            XMVectorSet(0, 0, 1, 0), /* Camera is looking forward in Z direction */
            XMVectorSet(0, 1, 0, 0)
        )),
        .projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(M_PI / 4.0f, 800.0f / 600.0f, 0.1f, 1000.0f))
    });
    matrices.bind(context, 0);

    CBuffer<Matrices> light(device, {
        .world = XMMatrixTranspose(XMMatrixIdentity()),
        .view = XMMatrixTranspose(XMMatrixLookAtLH(
            XMVectorSet(3.0f, 0, -3.0f, 0),
            XMVector3Normalize(XMVectorSet(-1.0f, 0, 1.0f, 0)),
            XMVectorSet(0, 1, 0, 0)
        )),
        .projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(M_PI / 4.0f, 800.0f / 600.0f, 0.1f, 1000.0f))
    });

    /* Initialize model transform to identity matrix */
    XMMATRIX modelTransform = XMMatrixIdentity();
    /* Transformation matrix for the model being displayed */
    CBuffer<Model> model(device, {
        .model = XMMatrixTranspose(XMMatrixRotationY(45.0f))
    });
    model.bind(context, 1);

    /* Time accumulator for rotation */
    f32 time = 0.0f;

    /* Simple textured triangle */
    Mesh<Vertex> mesh(
        device,
        {
            { XMFLOAT3(0, 0.5f, -0.1f), XMFLOAT4(1.0f, 0, 0, 0.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(0.5f, 0.0f)},      /* top front */
            { XMFLOAT3(0.5f, -0.5f, -0.1f), XMFLOAT4(0, 1.0f, 0, 0.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(1.0f, 1.0f)},  /* bottom right front*/
            { XMFLOAT3(-0.5f, -0.5f, -0.1f), XMFLOAT4(0, 0, 1.0f, 0.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(0.0f, 1.0f)}, /* bottom left front*/
            { XMFLOAT3(0, 0.5f, 0.1f), XMFLOAT4(1.0f, 0, 0, 0.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(0.5f, 0.0f)},       /* top back */
            { XMFLOAT3(0.5f, -0.5f, 0.1f), XMFLOAT4(0, 1.0f, 0, 0.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(1.0f, 1.0f)},   /* bottom right back*/
            { XMFLOAT3(-0.5f, -0.5f, 0.1f), XMFLOAT4(0, 0, 1.0f, 0.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(0.0f, 1.0f)},  /* bottom left back */
            { XMFLOAT3(0, 0.5f, -0.1f), XMFLOAT4(1.0f, 0, 0, 0.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f)},      /* top front, right */
            { XMFLOAT3(0, 0.5f, 0.1f), XMFLOAT4(1.0f, 0, 0, 0.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f)},       /* top back, right */
            { XMFLOAT3(0.5f, -0.5f, -0.1f), XMFLOAT4(0, 1.0f, 0, 0.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)}  /* bottom right front, right*/
        },
        {
            0, 1, 2, // front face
            3, 4, 5, // back face

            6, 7, 8 // right face
            //3, 1, 4, // right face
            
            //0, 3, 2, // left face
            //3, 2, 5  // left face
        }
    );
    mesh.use(context);

    app.run(
        [&](Application* appl)
        {
            f32 color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            matrices.bind(context, 0);
            graphics->resetRenderTarget();
            graphics->getBackBuffer()->clear(graphics, color);
            graphics->getDepthBuffer()->clearDepth(graphics);

            mesh.useAndDraw(context);

            graphics->setRenderTarget(nullptr, &shadowMap);
            shadowMap.clearDepth(graphics);
            light.bind(context, 0);
            mesh.useAndDraw(context);

            graphics->present();
        },
        [&](Application* appl, f32 dt)
        {
            /* Rotate matrix on the Y Axis */
            time += dt;
            modelTransform = XMMatrixRotationY(time / 100000000.0f); /* Time delta is in microseconds */

            model.setData({
                .model = XMMatrixTranspose(modelTransform)
            });
            model.bind(context, 1);
        }
    );
}
