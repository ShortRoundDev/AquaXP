#include "pch.h"
#include "Application.h"

using namespace AquaXP;
using namespace std;
using namespace DirectX;

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
    m_graphics(m_window.m_width, m_window.m_height, m_window.m_hwnd, m_window.m_fullscreen),
    m_keyboard(),
    m_keyboardStateTracker(),
    m_mouse(),
    m_mouseState(m_mouse.GetState()),
    m_actionBindings()
{
    m_mouse.SetWindow(m_window.m_hwnd);
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
            m_timer.Tick(this, [&](Application* app, f32 dt) -> void
                {
                    updateMouse();
                    updateKeyboard();
                    updateCamera(dt);
                    update(app, dt);
                });
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
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        Mouse::ProcessMessage(umsg, wparam, lparam);
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        Keyboard::ProcessMessage(umsg, wparam, lparam);
        break;
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
        Mouse::ProcessMessage(umsg, wparam, lparam);
        Keyboard::ProcessMessage(umsg, wparam, lparam);
        break;
    case WM_SYSKEYDOWN:
        if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
        {
            // Full screen;
        }
        Keyboard::ProcessMessage(umsg, wparam, lparam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEACTIVATE:
        // When you click to activate the window, we want Mouse to ignore that event.
        return MA_ACTIVATEANDEAT;
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

DirectX::Keyboard::State const& Application::getKeyboard() const
{
    return m_keyboardState;
}

DirectX::Mouse::State const& Application::getMouse() const
{
    return m_mouseState;
}

bool Application::isKeyDown(DirectX::Keyboard::Keys key) const
{
    return m_keyboardState.IsKeyDown(key);
}

bool Application::isKeyUp(DirectX::Keyboard::Keys key) const
{
    return m_keyboardState.IsKeyUp(key);
}

bool Application::keyPressed(DirectX::Keyboard::Keys key) const
{
    return m_keyboardStateTracker.IsKeyPressed(key);
}
bool Application::keyReleased(DirectX::Keyboard::Keys key) const
{
    return m_keyboardStateTracker.IsKeyReleased(key);
}

bool Application::tryPushCamera(std::shared_ptr<ICamera> camera)
{
    if (m_cameras.size() == 0)
    {
        return false;
    }
    // using current controller, with new camera
    auto const& context = m_cameras.top();
    m_cameras.push(CameraContext(context.m_controller, camera));
    return true;
}

bool Application::tryPushCameraController(std::shared_ptr<ICameraController> controller)
{
    if (m_cameras.size() == 0)
    {
        return false;
    }

    auto const& context = m_cameras.top();
    m_cameras.push(CameraContext(controller, context.m_camera));
    return true;
}

void Application::pushCameraContext(
    std::shared_ptr<ICameraController> cameraController,
    std::shared_ptr<ICamera> camera
)
{
    m_cameras.push(CameraContext(
        cameraController,
        camera
    ));
}

void Application::pushCameraContext(CameraContext const& cameraContext)
{
    m_cameras.push(cameraContext);
}

optional<CameraContext> Application::popCamera()
{
    if (m_cameras.size() == 0)
    {
        return nullopt;
    }

    auto camera = m_cameras.top();
    m_cameras.pop();
    return camera;
}

optional<CameraContext> Application::getCamera() const
{
    if (m_cameras.size() == 0)
    {
        return nullopt;
    }
    return m_cameras.top();
}

void Application::setMouseMode(Mouse::Mode mode)
{
    m_mouse.SetMode(mode);
    m_mouse.SetVisible(mode == Mouse::Mode::MODE_ABSOLUTE);
}

Mouse::Mode Application::getMouseMode() const
{
    return m_mouse.GetState().positionMode;
}

void Application::updateMouse()
{
    m_mouseState = m_mouse.GetState();
}

void Application::updateKeyboard()
{
    m_keyboardState = m_keyboard.GetState();
    m_keyboardStateTracker.Update(m_keyboardState);
}

void Application::updateCamera(f32 dt)
{
    if (m_cameras.size() == 0)
    {
        return;
    }
    auto& context = m_cameras.top();
    context.m_controller->update(*this, *context.m_camera, dt);
    context.m_camera->update(*this, dt);
}