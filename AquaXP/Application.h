#pragma once
#include <string>
#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>

#include "StepTimer.h"
#include "Graphics.h"
#include "ICamera.h"

namespace AquaXP
{
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
        AQUAXP_API DirectX::Keyboard const& getKeyboard() const;
        AQUAXP_API DirectX::Mouse const& getMouse() const;
        AQUAXP_API void pushCamera(std::shared_ptr<ICamera> camera);
        AQUAXP_API std::shared_ptr<ICamera> popCamera();
        AQUAXP_API std::shared_ptr<ICamera> getCamera();

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
        DirectX::Mouse m_mouse;

        std::stack<std::shared_ptr<ICamera>> m_cameras;
    };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
