#pragma once
#include <string>
#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>
#include <directxtk/GamePad.h>

#include "StepTimer.h"
#include "Graphics.h"
#include "ICamera.h"

namespace AquaXP
{

    enum DefaultActions
    {
        Forward = 1,
        Back = 2,
        Left = 3,
        Right = 4,
        Action = 5
    };

    constexpr i32 BeginActions = 16;

    struct Window
    {
        bool m_status;
        HINSTANCE m_instance;
        bool m_fullscreen;
        u16 m_width;
        u16 m_height;
        HWND m_hwnd;
        WCHAR const* m_title;
        bool m_vSync;
        bool m_enableTitlebar;
    };

    enum class AnalogInput
    {
        None,
        Keyboard,
        Mouse,
        GamePad
    };

    enum class MouseButton
    {
        None,
        Left,
        Right,
        Middle,
        x1,
        x2
    };

    enum class GamePadButton
    {
        None,
        A,
        B,
        X,
        Y,
        LeftStick,
        RightStick,
        LeftShoulder,
        RightShoulder,
        Back,
        Start,

        DPadUp,
        DPadDown,
        DPadRight,
        DPadLeft,

        LeftThumbAxisUp,
        LeftThumbAxisDown,
        LeftThumbAxisRight,
        LeftThumbAxisLeft,

        RightThumbUp,
        RightThumbDown,
        RightThumbRight,
        RightThumbLeft,

        TriggerLeft,
        TriggerRight,

    };

    using ActionBinding = std::variant<
        DirectX::Keyboard::Keys,
        GamePadButton,
        MouseButton
    >;

    constexpr sz g_maxActions = 256;
    constexpr sz g_maxGamePads = 4;

    class Application
    {
    public:
        AQUAXP_API Application(
            u16 width,
            u16 height,
            WCHAR const* title,
            bool vSync = false,
            bool fullscreen = false,
            bool enableTitlebar = true,
            bool fixedTimestep = false
        );
        AQUAXP_API ~Application() = default;

        AQUAXP_API void run(
            std::function<void(Application*)> draw,
            std::function<void(Application*, f32)> update
        );

        /* Getters/Setters */
        AQUAXP_API WCHAR const* getTitle() const;
        AQUAXP_API void setTitle(WCHAR const* title);

        AQUAXP_API bool isVsync() const;
        AQUAXP_API void setVsync(bool vSync);

        AQUAXP_API bool isFullscreen() const;
        AQUAXP_API void setFullscreen(bool fullscreen);

        AQUAXP_API bool isTitlebarEnabled() const;

        AQUAXP_API bool isFixedTimestep() const;

        AQUAXP_API HWND getHWND() const;
        AQUAXP_API HINSTANCE getInstance() const;

        AQUAXP_API u16 getWindowWidth() const;
        AQUAXP_API u16 getWindowHeight() const;

        AQUAXP_API u16 getClientWidth() const;
        AQUAXP_API u16 getClientHeight() const;

        AQUAXP_API Graphics& getGraphics();
        AQUAXP_API DirectX::Keyboard::State const& getKeyboard() const;
        AQUAXP_API DirectX::Keyboard::KeyboardStateTracker const& getKeyboardTracker() const;

        AQUAXP_API DirectX::Mouse::State const& getMouse() const;
        AQUAXP_API DirectX::Mouse::ButtonStateTracker const& getMouseStateTracker() const;

        AQUAXP_API std::optional<DirectX::GamePad::State const&> tryGetGamepad(i32 player = 0) const;
        AQUAXP_API std::optional<DirectX::GamePad::ButtonStateTracker const&> getGamepadStateTracker(i32 player = 0) const;

        AQUAXP_API void setAxisType(AnalogInput analogInput, i32 axis, i32 playerNum = 0);
        AQUAXP_API AnalogInput getAxisType(i32 axis, i32 playerNum = 0) const;
        AQUAXP_API DirectX::XMVECTOR getAxis(i32 axis, i32 playerNum = 0) const;

        AQUAXP_API bool isKeyDown(DirectX::Keyboard::Keys key) const;
        AQUAXP_API bool isKeyUp(DirectX::Keyboard::Keys key) const;
        AQUAXP_API bool isKeyPressed(DirectX::Keyboard::Keys key) const;
        AQUAXP_API bool isKeyReleased(DirectX::Keyboard::Keys key) const;

        AQUAXP_API bool isGamePadButtonDown(GamePadButton key, i32 playerNum = 0) const;
        AQUAXP_API bool isGamePadButtonUp(GamePadButton key, i32 playerNum = 0) const;
        AQUAXP_API bool isGamePadButtonPressed(GamePadButton key, i32 playerNum = 0) const;
        AQUAXP_API bool isGamePadButtonReleased(GamePadButton key, i32 playerNum = 0) const;

        AQUAXP_API bool isMouseButtonDown(MouseButton button) const;
        AQUAXP_API bool isMouseButtonUp(MouseButton button) const;
        AQUAXP_API bool isMouseButtonPressed(MouseButton button) const;
        AQUAXP_API bool isMouseButtonReleased(MouseButton button) const;

        AQUAXP_API bool tryPushCamera(std::shared_ptr<ICamera> camera);
        AQUAXP_API bool tryPushCameraController(std::shared_ptr<ICameraController> controller);

        AQUAXP_API void pushCameraContext(
            std::shared_ptr<ICameraController> cameraController,
            std::shared_ptr<ICamera> camera
        );
        AQUAXP_API void pushCameraContext(CameraContext const& cameraContext);

        AQUAXP_API std::optional<CameraContext> popCamera();
        AQUAXP_API std::optional<CameraContext> getCamera() const;

        AQUAXP_API void setMouseMode(DirectX::Mouse::Mode mode);
        AQUAXP_API DirectX::Mouse::Mode getMouseMode() const;

        AQUAXP_API bool tryBindAction(i32 action, ActionBinding binding, i32& existingAction, bool force = false);
        AQUAXP_API void clearAction(i32 action);

        AQUAXP_API bool isActionDown(i32 action, i32 playerNum = 0) const;
        AQUAXP_API bool isActionUp(i32 action, i32 playerNum = 0) const;
        AQUAXP_API bool isActionPressed(i32 action, i32 playerNum = 0) const;
        AQUAXP_API bool isActionReleased(i32 action, i32 playerNum = 0) const;

    private:
        /* Settings */

        /* Custom */
        StepTimer m_timer;
        bool m_fixedTimestep;

        /* Win32 */
        Window m_window;

        /* DirectX */
        Graphics m_graphics;

        /* DirectXTK */
        DirectX::Keyboard m_keyboard;
        DirectX::Keyboard::State m_keyboardState;
        DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;
        std::array<std::optional<ActionBinding>, g_maxActions> m_actionBindings;

        DirectX::Mouse m_mouse;
        DirectX::Mouse::State m_mouseState;
        DirectX::Mouse::ButtonStateTracker m_mouseButtonStateTracker;

        DirectX::GamePad m_gamePad;
        std::array<DirectX::GamePad::State, g_maxGamePads> m_gamePadState;
        std::array<DirectX::GamePad::ButtonStateTracker, g_maxGamePads> m_buttonStateTracker;

        std::array<std::pair<AnalogInput, AnalogInput>, g_maxGamePads> m_analogInputType;

        /* Cameras */
        std::stack<CameraContext> m_cameras;

        void updateMouse();
        void updateKeyboard();
        void updateGamePad();
        void updateCamera(f32 dt);

        std::optional<i32> findKeys(ActionBinding binding) const;

        bool gamePadButtonIsState(GamePadButton button, i32 playerNum, DirectX::GamePad::ButtonStateTracker::ButtonState checkState) const;
        bool mouseButtonIsState(MouseButton button, DirectX::Mouse::ButtonStateTracker::ButtonState checkState) const;
    };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
