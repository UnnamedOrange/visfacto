/**
 * @file TrayWindow.hpp
 * @author UnnamedOrange
 * @brief The hidden tray window.
 * @version 0.1
 * @date 2026-8-18
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <Windows.h>

#include "Autostart.hpp"
#include "Protection.hpp"
#include "Window.hpp"

namespace orange {
    /**
     * @brief The hidden tray window.
     */
    class TrayWindow : public Window {
        using Self = TrayWindow;
        using Super = Window;

    public:
        static constexpr wchar_t WINDOW_CLASS_NAME[] = L"visfacto";
        static constexpr wchar_t WINDOW_NAME[] = L"visfacto";

    public:
        static constexpr UINT WM_TRAYICON = WM_APP + 1;
        static constexpr UINT ID_TRAYICON = 1;

        static constexpr UINT_PTR ID_TIMER = 1;
        static constexpr UINT TIMER_INTERVAL = 2000;

    private:
        HINSTANCE const instance_handle;

    private:
        Protection protection;
        Autostart autostart{L"visfacto"};

    private:
        bool is_enabled = false;

    private:
        UINT taskbar_created_message{};

    public:
        explicit TrayWindow(HINSTANCE _instance_handle);

    protected:
        LRESULT window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    public:
        bool create_window_and_tray_icon();

    private:
        void on_tray_icon(UINT mouse_message);
        void on_timer();
        void on_settings_change();
        void on_destroy();

    private:
        void add_tray_icon();
        void remove_tray_icon();

        UINT track_popup_menu();
        void on_tray_menu_item(UINT cmd);
    };
} // namespace orange
