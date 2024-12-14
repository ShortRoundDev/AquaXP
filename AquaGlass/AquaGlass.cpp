// AquaGlass.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>

#include "AquaXP.h"

using namespace AquaXP;

struct alignas(16) Foo
{
    int x, y;
};

int main()
{
    Application app(800, 600, L"AquaGlass", false, false, true, true);
    auto clientRtv = app.getGraphics()->getBackBuffer();

    auto graphics = app.getGraphics();
    auto device = graphics->getDevice().Get();
    auto context = graphics->getContext().Get();

    Foo foo;
    foo.x = 1;
    foo.y = 2;

    CBuffer<Foo> cameraBuffer(device, foo);

    cameraBuffer.bind(context);

    app.run(
        [](Application* appl)
        {
        },
        [](Application* appl, f32 dt)
        {
        }
    );
}
