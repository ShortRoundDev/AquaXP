#include "pch.h"
#include "Application.h"

using namespace AquaXP;
using namespace std;

Application::Application(
    u16 width,
    u16 height,
    std::wstring const& title,
    bool vSync,
    bool fullscreen,
    bool enableTitlebar,
    bool fixedTimestep
) :
    m_width(width),
    m_height(height),
    m_title(title),
    m_vSync(vSync),
    m_fullscreen(fullscreen),
    m_enableTitlebar(enableTitlebar),
    m_fixedTimestep(fixedTimestep),
    m_graphics(nullptr)
{
    initWindow();
    initDirectX();
}



bool Application::initWindow()
{
    m_instance = GetModuleHandle(nullptr);
    WNDCLASSEX wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = m_instance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"AquaXP";
    wc.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&wc))
    {
        return false;
    }

    if (m_fullscreen)
    {
        m_width = GetSystemMetrics(SM_CXSCREEN);
        m_height = GetSystemMetrics(SM_CYSCREEN);
    }

    m_hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        wc.lpszClassName,
        m_title.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_width,
        m_height,
        nullptr,
        nullptr,
        m_instance,
        nullptr
    );

    if (!m_hwnd)
    {
        return false;
    }

    return true;
}

bool Application::initDirectX()
{

    m_graphics = std::make_unique<Graphics>(m_width, m_height, m_hwnd, m_fullscreen);
    return true;
}

void Application::run(void(*draw)(Application*), void(*update)(Application*, f32))
{
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    MSG msg = { 0 };
    f32 deltaTime = 0.0f;
    f32 accumulator = 0.0f;
    f32 timestep = 1.0f / 60.0f;

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_timer.Tick(this, update);
            draw(this);
        }
    }
}

u16 Application::getClientWidth() const
{
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    return static_cast<u16>(clientRect.right - clientRect.left);
}

u16 Application::getClientHeight() const
{
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    return static_cast<u16>(clientRect.bottom - clientRect.top);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, umsg, wparam, lparam);
    }

    return 0;
}

std::wstring const& Application::getTitle() const
{
    return m_title;
}

void Application::setTitle(std::wstring const& title)
{
    m_title = title;
}

bool Application::isVsync() const
{
    return m_vSync;
}

void Application::setVsync(bool vSync)
{
    m_vSync = vSync;
}

bool Application::isFullscreen() const
{
    return m_fullscreen;
}

void Application::setFullscreen(bool fullscreen)
{
    m_fullscreen = fullscreen;
}

bool Application::isTitlebarEnabled() const
{
    return m_enableTitlebar;
}

bool Application::isFixedTimestep() const
{
    return m_fixedTimestep;
}

HWND Application::getHWND() const
{
    return m_hwnd;
}

HINSTANCE Application::getInstance() const
{
    return m_instance;
}

u16 Application::getWindowWidth() const
{
    return m_width;
}

u16 Application::getWindowHeight() const
{
    return m_height;
}

Graphics* Application::getGraphics() const
{
    return m_graphics.get();
}