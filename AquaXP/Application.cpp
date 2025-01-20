#include "pch.h"
#include "Application.h"

using namespace AquaXP;
using namespace std;

Window initWindow(
    u16 width,
    u16 height,
    bool fullscreen,
    WCHAR const* title,
    bool enableTitlebar,
    bool vSync
)
{
    Window window;
    window.m_status = false;
    window.m_enableTitlebar = enableTitlebar;
    window.m_vSync = vSync;
    window.m_fullscreen = fullscreen;
    window.m_title = title;
    window.m_instance = GetModuleHandle(nullptr);

    WNDCLASSEX wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = window.m_instance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"AquaXP";
    wc.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&wc))
    {
        return window;
    }

    if (fullscreen)
    {
        window.m_width = GetSystemMetrics(SM_CXSCREEN);
        window.m_height = GetSystemMetrics(SM_CYSCREEN);
    }
    else
    {
        window.m_width = width;
        window.m_height = height;
    }

    window.m_hwnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        wc.lpszClassName,
        title,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        window.m_width,
        window.m_height,
        nullptr,
        nullptr,
        window.m_instance,
        nullptr
    );

    if (!window.m_hwnd)
    {
        return window;
    }

    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(result))
    {
        return window;
    }

    window.m_status = true;
    return window;
}

Application::Application(
    u16 width,
    u16 height,
    WCHAR const* title,
    bool vSync,
    bool fullscreen,
    bool enableTitlebar,
    bool fixedTimestep
) :
    m_window(initWindow(width, height, fullscreen, title, enableTitlebar, vSync)),
    m_fixedTimestep(fixedTimestep),
    m_graphics(m_window.m_width, m_window.m_height, m_window.m_hwnd, m_window.m_fullscreen)
{
}

void Application::run(
    std::function<void(Application*)> draw,
    std::function<void(Application*, f32)> update
)
{
    ShowWindow(m_window.m_hwnd, SW_SHOW);
    UpdateWindow(m_window.m_hwnd);

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
    GetClientRect(m_window.m_hwnd, &clientRect);
    return static_cast<u16>(clientRect.right - clientRect.left);
}

u16 Application::getClientHeight() const
{
    RECT clientRect;
    GetClientRect(m_window.m_hwnd, &clientRect);
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

WCHAR const* Application::getTitle() const
{
    return m_window.m_title;
}

void Application::setTitle(WCHAR const* title)
{
    m_window.m_title = title;
}

bool Application::isVsync() const
{
    return m_window.m_vSync;
}

void Application::setVsync(bool vSync)
{
    m_window.m_vSync = vSync;
}

bool Application::isFullscreen() const
{
    return m_window.m_fullscreen;
}

void Application::setFullscreen(bool fullscreen)
{
    m_window.m_fullscreen = fullscreen;
}

bool Application::isTitlebarEnabled() const
{
    return m_window.m_enableTitlebar;
}

bool Application::isFixedTimestep() const
{
    return m_fixedTimestep;
}

HWND Application::getHWND() const
{
    return m_window.m_hwnd;
}

HINSTANCE Application::getInstance() const
{
    return m_window.m_instance;
}

u16 Application::getWindowWidth() const
{
    return m_window.m_width;
}

u16 Application::getWindowHeight() const
{
    return m_window.m_height;
}

Graphics& Application::getGraphics()
{
    return m_graphics;
}