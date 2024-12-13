// AquaGlass.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>

#include "AquaXP.h"

using namespace AquaXP;

int main()
{
    Application app(800, 600, L"AquaGlass", false, false, true, true);
    auto clientRtv = app.getGraphics()->getBackBuffer();

    auto graphics = app.getGraphics();
    auto device = graphics->getDevice().Get();
    auto context = graphics->getContext().Get();

    CBuffer<int> cameraBuffer(device, 0);

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
