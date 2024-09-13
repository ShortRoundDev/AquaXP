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
        AQUAXP_API ~Application();

        AQUAXP_API void run(void(*draw)(Application*), void(*update)(Application*, f32));

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
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
