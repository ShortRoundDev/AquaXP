#pragma once
#include <string>

#include "StepTimer.h"
#include "Graphics.h"

namespace AquaXP
{
    class Application
    {
    public:
        AQUAXP_API Application(
            u16 width,
            u16 height,
            std::wstring const& title,
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
        AQUAXP_API std::wstring const& getTitle() const;
        AQUAXP_API void setTitle(std::wstring const& title);

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

        AQUAXP_API Graphics* getGraphics() const;

    private:
        /* Settings */
        
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

        bool initWindow();
        bool initDirectX();
    };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
