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
    Texture texture(device, context, L"Assets/Box.png");
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

    /* Initialize model transform to identity matrix */
    XMMATRIX modelTransform = XMMatrixIdentity();
    /* Transformation matrix for the model being displayed */
    CBuffer<Model> model(device, {
        .model = XMMatrixTranspose(modelTransform)
    });
    model.bind(context, 1);

    /* Time accumulator for rotation */
    f32 time = 0.0f;

    /* Simple textured triangle */
    Mesh<Vertex> mesh(
        device,
        {
            { XMFLOAT3(0, 0.5f, 0.0f), XMFLOAT4(1.0f, 0, 0, 1.0f), XMFLOAT2(0.5f, 0.0f) },
            { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0, 1.0f, 0, 1.0f), XMFLOAT2(1.0f, 1.0f) },
            { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0, 0, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) }
        },
        { 0, 1, 2 }
    );
    mesh.use(context);

    app.run(
        [&](Application* appl)
        {
            graphics->getBackBuffer()->clear(graphics);
            mesh.useAndDraw(context);
            graphics->present();
        },
        [&](Application* appl, f32 dt)
        {
            /* Rotate matrix on the Y Axis */
            time += dt;
            modelTransform = XMMatrixRotationY(time / 1000000.0f); /* Time delta is in microseconds */

            model.setData({
                .model = XMMatrixTranspose(modelTransform)
            });
            model.bind(context, 1);
        }
    );
}
