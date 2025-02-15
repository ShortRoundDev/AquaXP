#include "pch.h"
#include "Application.h"

using namespace AquaXP;
using namespace std;
using namespace DirectX;

template<class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

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
    m_keyboardState(m_keyboard.GetState()),
    m_keyboardStateTracker(),

    m_mouse(),
    m_mouseState(m_mouse.GetState()),
    m_mouseButtonStateTracker(),

    m_gamePad(),
    m_gamePadState(),
    m_buttonStateTracker(),

    m_actionBindings(),
    m_analogInputType()
{
    m_analogInputType[0] = make_pair(AnalogInput::Keyboard, AnalogInput::Mouse);
    for (i32 i = 1; i < g_maxGamePads; i++)
    {
        //TODO: COLLIN YOU WERE HERE
        //m_analogInputType[i] = make_pair(AnalogInput::GamePad, AnalogInput::
    }
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

DirectX::Keyboard::KeyboardStateTracker const& Application::getKeyboardTracker() const
{
    return m_keyboardStateTracker;
}

DirectX::Mouse::State const& Application::getMouse() const
{
    return m_mouseState;
}

DirectX::Mouse::ButtonStateTracker const& Application::getMouseStateTracker() const
{
    return m_mouseButtonStateTracker;
}

std::optional<DirectX::GamePad::State const&> Application::tryGetGamepad(i32 player = 0) const
{
    if (player < 0 || player >= g_maxGamePads)
    {
        return nullopt;
    }
    return make_optional(m_gamePadState[player]);
}

std::optional<DirectX::GamePad::ButtonStateTracker const&> Application::getGamepadStateTracker(i32 player = 0) const
{
    if (player < 0 || player >= g_maxGamePads)
    {
        return nullopt;
    }
    return make_optional(m_buttonStateTracker[player]);
}

bool Application::isKeyDown(DirectX::Keyboard::Keys key) const
{
    return m_keyboardState.IsKeyDown(key);
}

bool Application::isKeyUp(DirectX::Keyboard::Keys key) const
{
    return m_keyboardState.IsKeyUp(key);
}

bool Application::isKeyPressed(DirectX::Keyboard::Keys key) const
{
    return m_keyboardStateTracker.IsKeyPressed(key);
}
bool Application::isKeyReleased(DirectX::Keyboard::Keys key) const
{
    return m_keyboardStateTracker.IsKeyReleased(key);
}

bool Application::isGamePadButtonDown(GamePadButton button, i32 playerNum = 0) const
{
    return gamePadButtonIsState(button, playerNum, GamePad::ButtonStateTracker::ButtonState::HELD);
}

bool Application::isGamePadButtonUp(GamePadButton button, i32 playerNum = 0) const
{
    return gamePadButtonIsState(button, playerNum, GamePad::ButtonStateTracker::ButtonState::UP);
}

bool Application::isGamePadButtonPressed(GamePadButton button, i32 playerNum) const
{
    return gamePadButtonIsState(button, playerNum, GamePad::ButtonStateTracker::ButtonState::PRESSED);
}

bool Application::isGamePadButtonReleased(GamePadButton button, i32 playerNum) const
{
    return gamePadButtonIsState(button, playerNum, GamePad::ButtonStateTracker::ButtonState::RELEASED);
}

bool Application::isMouseButtonDown(MouseButton button) const
{
    return mouseButtonIsState(button, Mouse::ButtonStateTracker::ButtonState::HELD);
}

bool Application::isMouseButtonUp(MouseButton button) const
{
    return mouseButtonIsState(button, Mouse::ButtonStateTracker::ButtonState::UP);
}

bool Application::isMouseButtonPressed(MouseButton button) const
{
    return mouseButtonIsState(button, Mouse::ButtonStateTracker::ButtonState::PRESSED);
}

bool Application::isMouseButtonReleased(MouseButton button) const
{
    return mouseButtonIsState(button, Mouse::ButtonStateTracker::ButtonState::RELEASED);
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

std::optional<i32> Application::findKeys(ActionBinding binding) const
{
    for (i32 i = 0; i < g_maxActions; i++)
    {
        auto action = m_actionBindings[i];
        if (!action.has_value())
        {
            continue;
        }
        if(action.value() == binding)
        {
            return i;
        }
    }
    return nullopt;
}

bool Application::tryBindAction(i32 action, ActionBinding binding, i32& existingAction, bool force)
{
    if (action >= g_maxActions || action < 0)
    {
        return false;
    }
    auto existing = findKeys(binding);
    if (existing.has_value())
    {
        if (force)
        {
            clearAction(existing.value());
        }
        else
        {
            existingAction = existing.value();
            return false;
        }
    }

    m_actionBindings[action] = binding;
}

void Application::clearAction(i32 action)
{
    if (action < 0 || action >= g_maxActions)
    {
        return;
    }
    m_actionBindings[action] = nullopt;
}

bool Application::isActionDown(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
    if (!actionBinding.has_value())
    {
        return false;
    }
    return visit(
        overloaded
        {
            [&](Keyboard::Keys key) -> bool
            {
                return getKeyboard().IsKeyDown(key);
            },
            [&](GamePadButton button) -> bool
            {
                return isGamePadButtonDown(button, playerNum);
            },
            [&](MouseButton button) -> bool
            {
                return isMouseButtonDown(button);
            },
        },
        actionBinding.value()
    );
}

bool Application::isActionUp(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
    if (!actionBinding.has_value())
    {
        return false;
    }
    return visit(
        overloaded
        {
            [&](Keyboard::Keys key) -> bool
            {
                return getKeyboard().IsKeyUp(key);
            },
            [&](GamePadButton button) -> bool
            {
                return isGamePadButtonUp(button, playerNum);
            },
            [&](MouseButton button) -> bool
            {
                return isMouseButtonUp(button);
            },
        },
        actionBinding.value()
    );
}

bool Application::isActionPressed(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
    if (!actionBinding.has_value())
    {
        return false;
    }
    return visit(
        overloaded
        {
            [&](Keyboard::Keys key) -> bool
            {
                return getKeyboardTracker().IsKeyPressed(key);
            },
            [&](GamePadButton button) -> bool
            {
               return isGamePadButtonPressed(button, playerNum);
            },
            [&](MouseButton button) -> bool
            {
                return isMouseButtonPressed(button);
            }
        },
        actionBinding.value()
    );
}

bool Application::isActionReleased(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
    if (!actionBinding.has_value())
    {
        return false;
    }
    return visit(
        overloaded
        {
            [&](Keyboard::Keys key) -> bool
            {
                return getKeyboardTracker().IsKeyReleased(key);
            },
            [&](GamePadButton button) -> bool
            {
                return isGamePadButtonReleased(button, playerNum);
            },
            [&](MouseButton button) -> bool
            {
                return isMouseButtonReleased(button);
            },
        },
        actionBinding.value()
    );
}

void Application::updateMouse()
{
    m_mouseState = m_mouse.GetState();
    m_mouseButtonStateTracker.Update(m_mouseState);
}

void Application::updateKeyboard()
{
    m_keyboardState = m_keyboard.GetState();
    m_keyboardStateTracker.Update(m_keyboardState);
}

void Application::updateGamePad()
{
    for (i32 i = 0; i < g_maxGamePads; i++)
    {
        m_gamePadState[i] = m_gamePad.GetState(i);
        m_buttonStateTracker[i].Update(m_gamePadState[i]);
    }
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

bool Application::gamePadButtonIsState(GamePadButton button, i32 playerNum, GamePad::ButtonStateTracker::ButtonState checkState) const
{
    if (playerNum < 0 || playerNum > g_maxGamePads)
    {
        return false;
    }

    GamePad::ButtonStateTracker const& tracker = m_buttonStateTracker[playerNum];
    GamePad::ButtonStateTracker::ButtonState state;
    switch (button)
    {
    case GamePadButton::None:
        return false;
        break;
    case GamePadButton::A:
        state = tracker.a;
        break;
    case GamePadButton::B:
        state = tracker.b;
        break;
    case GamePadButton::X:
        state = tracker.x;
        break;
    case GamePadButton::Y:
        state = tracker.y;
        break;
    case GamePadButton::LeftStick:
        state = tracker.leftStick;
        break;
    case GamePadButton::RightStick:
        state = tracker.rightStick;
        break;
    case GamePadButton::LeftShoulder:
        state = tracker.leftShoulder;
        break;
    case GamePadButton::RightShoulder:
        state = tracker.rightShoulder;
        break;
    case GamePadButton::Back:
        state = tracker.back;
        break;
    case GamePadButton::Start:
        state = tracker.start;
        break;
    case GamePadButton::DPadUp:
        state = tracker.dpadUp;
        break;
    case GamePadButton::DPadDown:
        state = tracker.dpadUp;
        break;
    case GamePadButton::DPadRight:
        state = tracker.dpadRight;
        break;
    case GamePadButton::DPadLeft:
        state = tracker.dpadLeft;
        break;
    case GamePadButton::LeftThumbAxisUp:
        state = tracker.leftStickUp;
        break;
    case GamePadButton::LeftThumbAxisDown:
        state = tracker.leftStickDown;
        break;
    case GamePadButton::LeftThumbAxisRight:
        state = tracker.leftStickRight;
        break;
    case GamePadButton::LeftThumbAxisLeft:
        state = tracker.leftStickLeft;
        break;
    case GamePadButton::RightThumbUp:
        state = tracker.rightStickUp;
        break;
    case GamePadButton::RightThumbDown:
        state = tracker.rightStickDown;
        break;
    case GamePadButton::RightThumbRight:
        state = tracker.rightStickRight;
        break;
    case GamePadButton::RightThumbLeft:
        state = tracker.rightStickLeft;
        break;
    case GamePadButton::TriggerLeft:
        state = tracker.leftTrigger;
        break;
    case GamePadButton::TriggerRight:
        state = tracker.rightTrigger;
        break;
    }
    return state == checkState;
}

bool Application::mouseButtonIsState(MouseButton button, Mouse::ButtonStateTracker::ButtonState checkState) const
{
    Mouse::ButtonStateTracker const& tracker = m_mouseButtonStateTracker;
    switch (button)
    {
    case MouseButton::None:
        return false;
    case MouseButton::Left:
        return tracker.leftButton == checkState;
    case MouseButton::Right:
        return tracker.rightButton == checkState;
    case MouseButton::Middle:
        return tracker.middleButton == checkState;
    case MouseButton::x1:
        return tracker.xButton1 == checkState;
    case MouseButton::x2:
        return tracker.xButton2 == checkState;
    }
}