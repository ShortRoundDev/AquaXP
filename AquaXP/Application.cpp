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
    m_axisInputType(),

    m_sensitivity()
{
    m_axisInputType[0] = make_pair(AxisInput::Keyboard, AxisInput::Mouse);
    m_sensitivity[0] = make_pair(0.5f, 0.005f);
    for (i32 i = 1; i < g_maxGamePads; i++)
    {
        m_axisInputType[i] = make_pair(AxisInput::GamePad, AxisInput::GamePad);
        m_sensitivity[i] = make_pair(1.0f, 0.5f);
    }
    m_mouse.SetWindow(m_window.m_hwnd);
}

void Application::run(
    function<void(Application*)> draw,
    function<void(Application*, f32)> update
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

optional<DirectX::GamePad::State> Application::tryGetGamepad(i32 player) const
{
    if (player < 0 || player >= g_maxGamePads)
    {
        return nullopt;
    }
    return make_optional(m_gamePadState[player]);
}

optional<DirectX::GamePad::ButtonStateTracker> Application::getGamepadStateTracker(i32 player) const
{
    if (player < 0 || player >= g_maxGamePads)
    {
        return nullopt;
    }
    return make_optional(m_buttonStateTracker[player]);
}

void Application::setAxisType(AxisInput axisInput, i32 axis, i32 playerNum)
{
    if (playerNum < 0 || playerNum >= g_maxGamePads || (axis != 0 && axis != 1))
    {
        return;
    }
    auto current = m_axisInputType[playerNum];
    if (axis == 0)
    {
        m_axisInputType[playerNum] = make_pair(
            axisInput,
            current.second
        );
    }
    else
    {
        m_axisInputType[playerNum] = make_pair(
            current.first,
            axisInput
        );
    }
}

AxisInput Application::getAxisType(i32 axis, i32 playerNum) const
{
    if ((axis != 0 && axis != 1) || playerNum >= g_maxGamePads)
    {
        return AxisInput::None;
    }
    auto inputs = m_axisInputType[playerNum];
    if (axis == 0)
    {
        return inputs.first;
    }
    else
    {
        return inputs.second;
    }
}

DirectX::XMVECTOR Application::getAxis(i32 axis, i32 playerNum) const
{
    if (playerNum < 0 || playerNum >= g_maxGamePads || (axis != 0 && axis != 1))
    {
        return XMVectorSet(0, 0, 0, 0);
    }
    auto axisType = getAxisType(axis, playerNum);
    switch (axisType)
    {
    case AxisInput::None:
    {
        return XMVectorSet(0, 0, 0, 0);
    }
    case AxisInput::Keyboard:
    {
        f32 x = 0, y = 0;
        if (isActionDown(DefaultActions::Forward))
        {
            y += 1;
        }
        if (isActionDown(DefaultActions::Back))
        {
            y -= 1;
        }
        if (isActionDown(DefaultActions::Right))
        {
            x += 1;
        }
        if (isActionDown(DefaultActions::Left))
        {
            x -= 1;
        }

        return XMVectorSet(x, 0, y, 0);
    }
    case AxisInput::GamePad:
    {
        auto sticks = m_gamePadState[playerNum].thumbSticks;
        return XMVectorSet(
            axis == 0 ? sticks.leftX : sticks.rightX,
            0,
            axis == 0 ? sticks.leftY : sticks.rightY,
            0
        );
    }
    case AxisInput::Mouse:
    {
        return XMVectorSet(
            static_cast<f32>(m_mouseState.x),
            0,
            static_cast<f32>(m_mouseState.y),
            0
        );
    }
    default:
    {
        return XMVectorSet(0, 0, 0, 0);
    }
    }
}

DirectX::XMVECTOR Application::getMove(i32 playerNum, i32 axis) const
{
    if (playerNum < 0 || playerNum >= g_maxGamePads || (axis != 0 && axis != 1))
    {
        return XMVectorSet(0, 0, 0, 0);
    }

    auto vector = getAxis(axis, playerNum);
    auto axisType = getAxisType(axis, playerNum);
    auto sensitivity = axis == 0
        ? m_sensitivity[playerNum].first
        : m_sensitivity[playerNum].second;
    switch (axisType)
    {
    case AxisInput::None:
    {
        return XMVectorSet(0, 0, 0, 0);
    }
    case AxisInput::Keyboard:
    {
        return XMVector3Normalize(vector);
    }
    case AxisInput::GamePad:
    {
        return XMVector3ClampLength(vector, 0, sensitivity) / sensitivity;
    }
    case AxisInput::Mouse:
    {
        auto adjustedSensitivity = sensitivity / 100.0f;
        return XMVector3ClampLength(XMVectorMultiply(vector, XMVectorSet(1, 0, -1, 0)), 0, adjustedSensitivity) / adjustedSensitivity;
    }
    }
    return XMVectorSet(0, 0, 0, 0);
}

DirectX::XMVECTOR Application::getLook(i32 playerNum, i32 axis) const
{
    if (playerNum < 0 || playerNum >= g_maxGamePads || (axis != 0 && axis != 1))
    {
        return XMVectorSet(0, 0, 0, 0);
    }

    auto axisType = getAxisType(axis, playerNum);

    auto sensitivity = axis == 0
        ? m_sensitivity[playerNum].first
        : m_sensitivity[playerNum].second;

    switch (axisType)
    {
    case AxisInput::None:
    {
        return XMVectorSet(0, 0, 0, 0);
    }
    case AxisInput::Keyboard:
    {
        f32 yaw = 0, pitch = 0;
        if (axis == 1)
        {
            if (isActionDown(DefaultActions::LookUp, playerNum))
            {
                pitch -= sensitivity;
            }
            if (isActionDown(DefaultActions::LookDown, playerNum))
            {
                pitch += sensitivity;
            }
            if (isActionDown(DefaultActions::LookRight, playerNum))
            {
                yaw += sensitivity;
            }
            if (isActionDown(DefaultActions::LookLeft, playerNum))
            {
                yaw -= sensitivity;
            }
            return XMVectorSet(yaw, 0, pitch, 0);
        }
        else
        {
            return getAxis(axis, playerNum) * sensitivity; // getAxis() returns a range of -1, 1 for keyboard, so multiply by sensitivity to get the same range
        }
    }
    case AxisInput::GamePad:
    {
        return XMVector3ClampLength(getAxis(axis, playerNum), 0, sensitivity) / sensitivity;
    }
    case AxisInput::Mouse:
    {
        return getAxis(axis, playerNum) * sensitivity;
    }
    }
    return XMVectorSet(0, 0, 0, 0);

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

bool Application::isGamePadButtonDown(GamePadButton button, i32 playerNum) const
{
    return gamePadButtonIsState(button, playerNum, GamePad::ButtonStateTracker::ButtonState::HELD);
}

bool Application::isGamePadButtonUp(GamePadButton button, i32 playerNum) const
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

bool Application::tryPushCamera(shared_ptr<ICamera> camera)
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

bool Application::tryPushCameraController(shared_ptr<ICameraController> controller)
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
    shared_ptr<ICameraController> cameraController,
    shared_ptr<ICamera> camera
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

optional<i32> Application::findKeys(ActionBinding binding) const
{
    for (i32 i = 0; i < g_maxActions; i++)
    {
        auto action = m_actionBindings[i];
        if (auto const* key = get_if<Keyboard::Keys>(&action))
        {
            /* Keyboard::Keys::None is the universal "none" */
            if (*key == Keyboard::Keys::None) {
                return nullopt;
            }
        }
        else if (auto const* button = get_if<GamePadButton>(&action))
        {
            if (*button == GamePadButton::None)
            {
                return nullopt;
            }
        }
        else if (auto const* mouse = get_if<MouseButton>(&action))
        {
            if (*mouse == MouseButton::None)
            {
                return nullopt;
            }
        }
        else if(action == binding)
        {
            return i;
        }
    }
    return nullopt;
}

bool Application::tryBindAction(i32 action, ActionBinding binding, bool force, i32* existingAction)
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
            if (existingAction != nullptr)
            {
                *existingAction = existing.value();
            }
            return false;
        }
    }

    m_actionBindings[action] = binding;
    return true;
}

void Application::clearAction(i32 action)
{
    if (action < 0 || action >= g_maxActions)
    {
        return;
    }
    m_actionBindings[action] = Keyboard::None;
}

bool Application::isActionDown(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
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
        actionBinding
    );
}

bool Application::isActionUp(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
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
        actionBinding
    );
}

bool Application::isActionPressed(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
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
        actionBinding
    );
}

bool Application::isActionReleased(i32 action, i32 playerNum) const
{
    if (action < 0 || action >= g_maxActions)
    {
        return false;
    }
    auto actionBinding = m_actionBindings[action];
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
        actionBinding
    );
}

void Application::setAxisSensitivity(f32 sensitivity, i32 axis, i32 playerNum = 0)
{
    if (playerNum < 0 || playerNum > g_maxGamePads || (axis != 0 && axis != 1))
    {
        return;
    }
    auto current = m_sensitivity[playerNum];
    if (axis == 0)
    {
        m_sensitivity[playerNum] = make_pair(
            sensitivity,
            current.second
        );
    }
    else
    {
        m_sensitivity[playerNum] = make_pair(
            current.first,
            sensitivity
        );
    }
}

f32 Application::getAxisSensitivity(i32 axis, i32 playerNum = 0)
{
    if (playerNum < 0 || playerNum > g_maxGamePads || (axis != 0 && axis != 1))
    {
        return -1.0f;
    }
    return axis == 0
        ? m_sensitivity[playerNum].first
        : m_sensitivity[playerNum].second;

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
    GamePad::ButtonStateTracker::ButtonState state = static_cast<GamePad::ButtonStateTracker::ButtonState>(0xffffffff); // invalid state, returns false by default. This is just to clear a warning from MSVC
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
    default:
    {
        return false;
    }
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
    return false;
}