#include "pch.h"
#include "Application.h"

using namespace AquaXP;
using namespace std;

class Application::impl
{
public:
    impl(
        Application* application,
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

    /* Window info */
    wstring m_title;
    bool m_vSync;
    bool m_fullscreen;
    bool m_enableTitlebar;
    bool m_fixedTimestep;

    /* Custom */
    StepTimer m_timer;

    /* Win32 */
    HWND m_hwnd;
    HINSTANCE m_instance;
    u16 m_width;
    u16 m_height;

    /* DirectX */
    std::unique_ptr<Graphics> m_graphics;

    /* Initializers */
    using Initializer = bool(Application::impl::*)(Application* application);
    bool initWaterfall(
        Application* application,
        std::initializer_list<Initializer> initializers
    )
    {
        for (auto initializer : initializers)
        {
            if (!(this->*initializer)(application))
            {
                return false;
            }
        }

        return true;
    }

    bool initWindow()
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

    bool initDirectX()
    {
        m_graphics = std::make_unique<Graphics>(m_width, m_height, m_hwnd, m_fullscreen);
        return true;
    }

    void run(Application* application, void(*draw)(Application*), void(*update)(Application*, f32))
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
                m_timer.Tick(application, update);
                draw(application);
            }
        }
    }
};

Application::Application(
    u16 width,
    u16 height,
    std::wstring const& title,
    bool vSync,
    bool fullscreen,
    bool enableTitlebar,
    bool fixedTimestep
) : m_pimpl(std::make_unique<Application::impl>(
        this,
        width,
        height,
        title,
        vSync,
        fullscreen,
        enableTitlebar,
        fixedTimestep
    ))
{
}

Application::~Application() = default;

void Application::run(void(*draw)(Application*), void(*update)(Application*, f32))
{
    m_pimpl->run(this, draw, update);
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

wchar_t const* Application::getTitle() const
{
    return m_pimpl->m_title.c_str();
}

void Application::setTitle(std::wstring const& title)
{
    m_pimpl->m_title = title;
}

bool Application::isVsync() const
{
    return m_pimpl->m_vSync;
}

void Application::setVsync(bool vSync)
{
    m_pimpl->m_vSync = vSync;
}

bool Application::isFullscreen() const
{
    return m_pimpl->m_fullscreen;
}

void Application::setFullscreen(bool fullscreen)
{
    m_pimpl->m_fullscreen = fullscreen;
}

bool Application::isTitlebarEnabled() const
{
    return m_pimpl->m_enableTitlebar;
}

bool Application::isFixedTimestep() const
{
    return m_pimpl->m_fixedTimestep;
}

HWND Application::getHWND() const
{
    return m_pimpl->m_hwnd;
}

HINSTANCE Application::getInstance() const
{
    return m_pimpl->m_instance;
}

u16 Application::getWidth() const
{
    return m_pimpl->m_width;
}

u16 Application::getHeight() const
{
    return m_pimpl->m_height;
}

Graphics* Application::getGraphics() const
{
    return m_pimpl->m_graphics.get();
}