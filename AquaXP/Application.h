#pragma once
#include <string>

#include "StepTimer.h"
#include "Graphics.h"

#include "Types.h"

namespace AquaXP
{
    class Application
    {
    public:
        Application(
            u16 width,
            u16 height,
            std::wstring const& title,
            bool vSync = false,
            bool fullscreen = false,
            bool enableTitlebar = true,
            bool fixedTimestep = false
        ) : m_width(width),
            m_height(height),
            m_title(title),
            m_vSync(vSync),
            m_fullscreen(fullscreen),
            m_enableTitlebar(enableTitlebar),
            m_fixedTimestep(fixedTimestep),

            m_hwnd(),
            m_instance()
        {
            initWaterfall({
                &Application::initWindow,
                &Application::initDirectX
            });
        }

        ~Application();

        void run(void(*draw)(Application*), void(*update)(Application*, f32));

        /* Getters/Setters */
        std::wstring const& getTitle() const;
        void setTitle(std::wstring const& title);

        bool isVsync() const;
        void setVsync(bool vSync);

        bool isFullscreen() const;
        void setFullscreen(bool fullscreen);

        bool isTitlebarEnabled() const;

        bool isFixedTimestep() const;

        HWND getHWND() const;
        HINSTANCE getInstance() const;

        u16 getWidth() const;
        u16 getHeight() const;

        Graphics* getGraphics() const;

    private:
        /* Settings */
        std::wstring const* m_title;
        bool m_vSync;
        bool m_fullscreen;
        bool m_enableTitlebar;
        bool m_fixedTimestep;

        /* Custom */
        StepTimer m_timer;

        /* Windows */
        HWND m_hwnd;
        HINSTANCE m_instance;
        u16 m_width;
        u16 m_height;

        /* DirectX */
        std::unique_ptr<Graphics> m_graphics;

        /* Initializers */
        using Initializer = bool(Application::*)();
        bool initWaterfall(std::initializer_list<Initializer> initializers);

        bool initWindow();
        bool initDirectX();
    };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
