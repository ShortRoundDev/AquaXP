// AquaGlass.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>

#include "AquaXP.h"

using namespace AquaXP;

int main()
{
    Application app(800, 600, L"AquaGlass", false, false, true, true);
    app.run(
        [](Application* appl)
        {
        },
        [](Application* appl, f32 dt)
        {
        }
    );
}
